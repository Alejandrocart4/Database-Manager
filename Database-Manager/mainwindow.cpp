#include "MainWindow.h"
#include "ResultTableWidget.h"
#include "SqlConsoleWidget.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSplitter>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QRegularExpression>


static QString typeOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole).toString(); }
static QString dbOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole+1).toString(); }
static QString nameOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole+2).toString(); }

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_meta(&m_session)
{
    buildUi();

    QString err;
    if(!m_session.openWithDsn(
            "DRIVER={MariaDB ODBC 3.2 Driver};"
            "SERVER=localhost;"
            "PORT=3306;"
            "DATABASE=database-manager;"
            "UID=root;"
            "PWD=cartagena65;",
            &err))

    {
        QMessageBox::critical(this, "Error de conexión", err);
        return;
    }

    loadDatabases();
}

void MainWindow::buildUi()
{
    m_tree = new QTreeWidget;
    m_tree->setHeaderLabel("Servidor");

    m_results = new ResultTableWidget;

    // Este lo dejas como DDL (solo lectura)
    m_ddl = new QPlainTextEdit;
    m_ddl->setReadOnly(true);

    // Consola SQL (incluye resaltado + historial + estado)
    m_console = new SqlConsoleWidget;

    // Panel derecho: resultados arriba, DDL medio, SQL abajo
    auto* right = new QSplitter(Qt::Vertical);
    right->addWidget(m_results);
    right->addWidget(m_ddl);
    right->addWidget(m_console);

    auto* main = new QSplitter(Qt::Horizontal);
    main->addWidget(m_tree);
    main->addWidget(right);

    auto* root = new QWidget;
    auto* lay = new QVBoxLayout(root);
    lay->addWidget(main);

    setCentralWidget(root);

    connect(m_tree, &QTreeWidget::itemExpanded, this, [&](QTreeWidgetItem* i){
        if(typeOf(i)=="db" && i->childCount()==0)
            loadDbChildren(nameOf(i));
    });

    connect(m_tree, &QTreeWidget::itemSelectionChanged, this, &MainWindow::showDdlForNode);

    // Ejecutar SQL
    connect(m_console, &SqlConsoleWidget::executeRequested, this, &MainWindow::executeSql);
}

void MainWindow::loadDatabases()
{
    m_tree->clear();
    auto* root = new QTreeWidgetItem(m_tree);
    root->setText(0,"MariaDB");

    for(const auto& db : m_meta.listDatabases()){
        auto* d = new QTreeWidgetItem(root);
        d->setText(0,db);
        d->setData(0,Qt::UserRole,"db");
        d->setData(0,Qt::UserRole+2,db);
        d->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    }
    root->setExpanded(true);
}

void MainWindow::loadDbChildren(const QString& db)
{
    auto* root = m_tree->topLevelItem(0);
    QTreeWidgetItem* dbNode=nullptr;

    for(int i=0;i<root->childCount();++i)
        if(root->child(i)->text(0)==db) dbNode=root->child(i);

    auto* tables = new QTreeWidgetItem(dbNode);
    tables->setText(0,"Tablas");

    for(const auto& t : m_meta.listTables(db)){
        auto* it = new QTreeWidgetItem(tables);
        it->setText(0,t);
        it->setData(0,Qt::UserRole,"table");
        it->setData(0,Qt::UserRole+1,db);
        it->setData(0,Qt::UserRole+2,t);
    }
}

void MainWindow::refreshDatabaseNode(const QString& dbName)
{
    auto* root = m_tree->topLevelItem(0);
    if (!root) return;

    QTreeWidgetItem* dbNode = nullptr;
    for (int i = 0; i < root->childCount(); ++i) {
        if (root->child(i)->text(0) == dbName) {
            dbNode = root->child(i);
            break;
        }
    }
    if (!dbNode) return;

    // Limpia hijos actuales
    dbNode->takeChildren();

    // Vuelve a cargar (esto recrea “Tablas” y sus items)
    loadDbChildren(dbName);

    // Mantén expandido
    dbNode->setExpanded(true);
}


void MainWindow::showDdlForNode()
{
    auto* it = m_tree->currentItem();
    if(!it) return;

    if(typeOf(it)=="table")
        m_ddl->setPlainText(m_meta.showCreateTable(dbOf(it),nameOf(it)));
}

static QString normalizeShowTablesFrom(QString sql)
{
    // Captura SHOW TABLES FROM <identificador> donde el identificador puede venir
    // como `db`, "db", 'db' o incluso ""db"".
    static const QRegularExpression re(
        R"((?i)^\s*show\s+tables\s+from\s+(.+?)\s*;?\s*$)"
        );

    auto m = re.match(sql);
    if (!m.hasMatch())
        return sql;

    QString db = m.captured(1).trimmed();

    // Quita ; final si venía pegado
    if (db.endsWith(";"))
        db.chop(1);

    // Limpia comillas dobles duplicadas: ""db"" -> "db"
    while (db.startsWith("\"\"") && db.endsWith("\"\"") && db.size() >= 4) {
        db = db.mid(2, db.size() - 4);
        db = db.trimmed();
    }

    // Quita delimitadores conocidos
    if ((db.startsWith('`') && db.endsWith('`')) ||
        (db.startsWith('"') && db.endsWith('"')) ||
        (db.startsWith('\'') && db.endsWith('\''))) {
        db = db.mid(1, db.size() - 2).trimmed();
    }

    return QString("SHOW TABLES FROM `%1`;").arg(db.replace("`",""));
}

static QString firstTokenUpper(const QString& sql)
{
    QString s = sql.trimmed();
    if (s.isEmpty()) return {};

    // -- comment
    while (s.startsWith("--")) {
        int nl = s.indexOf('\n');
        if (nl < 0) return {};
        s = s.mid(nl + 1).trimmed();
    }

    // /* comment */
    while (s.startsWith("/*")) {
        int end = s.indexOf("*/");
        if (end < 0) return {};
        s = s.mid(end + 2).trimmed();
    }

    // primer "token"
    int i = 0;
    while (i < s.size() && !s[i].isSpace() && s[i] != ';') i++;
    return s.left(i).toUpper();
}

static bool isDbLevelDdl(const QString& sql)
{
    const QString t = firstTokenUpper(sql);
    if (t == "CREATE" || t == "DROP" || t == "ALTER" || t == "RENAME") {
        const QString u = sql.trimmed().toUpper();

        return u.contains(" DATABASE ") || u.contains(" SCHEMA ");
    }
    return false;
}

static bool isTableLevelDdlOrDmlThatAffectsMetadata(const QString& sql)
{
    const QString t = firstTokenUpper(sql);
    if (t == "CREATE" || t == "DROP" || t == "ALTER" || t == "RENAME" || t == "TRUNCATE")
        return true;

    return false;
}

void MainWindow::executeSql(const QString& sql)
{

    QString selectedDb;
    if (auto* it = m_tree->currentItem()) {
        const QString type = typeOf(it);

        if (type == "db") {
            selectedDb = nameOf(it);
        } else if (type == "table") {
            selectedDb = dbOf(it);
        }
    }

    QString err;
    const bool ok = m_results->setQuery(sql, "odbc_conn", &err);

    if (!ok) {
        m_console->setStatusError(err);
        return;
    }

    m_console->setStatusOk("OK");

    // Refrescar metadatos SOLO si hubo cambios estructurales
    if (isDbLevelDdl(sql)) {
        loadDatabases();

        if (!selectedDb.isEmpty()) {
            auto* root = m_tree->topLevelItem(0);
            if (root) {
                for (int i=0; i<root->childCount(); ++i) {
                    if (root->child(i)->text(0) == selectedDb) {
                        root->child(i)->setExpanded(true);
                        break;
                    }
                }
            }
        }
        return;
    }

    if (isTableLevelDdlOrDmlThatAffectsMetadata(sql)) {
        if (selectedDb.isEmpty()) {
            loadDatabases();
        } else {
            refreshDatabaseNode(selectedDb);
        }
    }
}
