#include "MainWindow.h"
#include "ResultTableWidget.h"
#include "SqlConsoleWidget.h"
#include "LoginDialog.h"

#include <QApplication>
#include <QClipboard>
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
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QDir>
#include <QDateTime>




static QString typeOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole).toString(); }
static QString dbOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole+1).toString(); }
static QString nameOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole+2).toString(); }

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_meta(&m_session)
{
    buildUi();

    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tree, &QWidget::customContextMenuRequested, this, [this](const QPoint& pos){
        QTreeWidgetItem* it = m_tree->itemAt(pos);
        if (!it) return;

        QMenu menu;

        // Root "MariaDB": toggle system schemas
        if (it == m_tree->topLevelItem(0)) {
            QAction* toggleSystem = menu.addAction(
                m_showSystemSchemas
                    ? "Ocultar bases del sistema (information_schema)"
                    : "Mostrar bases del sistema (information_schema)"
                );

            QAction* chosen = menu.exec(m_tree->viewport()->mapToGlobal(pos));
            if (chosen == toggleSystem) {
                m_showSystemSchemas = !m_showSystemSchemas;
                loadDatabases();
            }
            return;
        }

        const QString t = typeOf(it);

        // Acciones DDL aplicables (por ahora db y table; puedes extender a view/trigger luego)
        const bool canDdl = (t == "db" || t == "table");

        QAction* actExport = nullptr;
        QAction* actCopy   = nullptr;
        QAction* actToSql  = nullptr;

        if (canDdl) {
            actExport = menu.addAction("Exportar DDL (archivo)");
            actCopy   = menu.addAction("Copiar DDL");
            actToSql  = menu.addAction("Enviar DDL a la consola SQL");
        }

        QAction* chosen = menu.exec(m_tree->viewport()->mapToGlobal(pos));
        if (!chosen) return;

        const QString ddl = ddlForItem(it);
        if (ddl.trimmed().isEmpty()) {
            QMessageBox::information(this, "DDL", "No hay DDL disponible para este objeto.");
            return;
        }

        if (chosen == actCopy) {
            QApplication::clipboard()->setText(ddl);
            m_console->setStatusOk("DDL copiado al portapapeles.");
            return;
        }

        if (chosen == actToSql) {
            m_console->setSql(ddl);
            m_console->setStatusOk("DDL enviado a la consola. Modifica y ejecuta el script si necesitas cambios.");
            return;
        }

        if (chosen == actExport) {
            const QString path = exportFilePathForItem(it);

            QFile f(path);
            if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                QMessageBox::critical(this, "Error", "No se pudo escribir el archivo:\n" + path);
                return;
            }

            QTextStream out(&f); // Qt6: UTF-8 por defecto
            out << ddl;
            if (!ddl.endsWith('\n')) out << "\n";
            f.close();

            QMessageBox::information(this, "Exportar DDL", "DDL exportado en:\n" + path);
            return;
        }
    });

    LoginDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
        // Si cancela, cierra app o deja ventana vacía.
        // Lo más claro: cerrar.
        close();
        return;
    }

    QString err;
    if (!m_session.openWithDsn(dlg.dsn(), &err)) {
        QMessageBox::critical(this, "Error de conexión", err);
        close();
        return;
    }

    loadDatabases();
}




void MainWindow::buildUi()
{
    m_tree = new QTreeWidget;
    m_tree->setHeaderLabel("Servidor");

    m_results = new ResultTableWidget;

    m_ddl = new QPlainTextEdit;
    m_ddl->setReadOnly(true);

    m_console = new SqlConsoleWidget;

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
    connect(m_console, &SqlConsoleWidget::executeRequested, this, &MainWindow::executeSql);
}



void MainWindow::loadDatabases()
{
    const QStringList systemSchemas = {
        "information_schema",
        "performance_schema",
        "mysql",
        "sys"
    };

    m_tree->clear();

    auto* root = new QTreeWidgetItem(m_tree);
    root->setText(0,"MariaDB");
    root->setExpanded(true);

    const auto dbs = m_meta.listDatabases();
    for (const auto& db : dbs) {
        if (!m_showSystemSchemas && systemSchemas.contains(db, Qt::CaseInsensitive))
            continue;

        auto* d = new QTreeWidgetItem(root);
        d->setText(0,db);
        d->setData(0,Qt::UserRole,"db");
        d->setData(0,Qt::UserRole+2,db);
        d->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    }
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

    m_ddl->setPlainText(ddlForItem(it));
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


QString MainWindow::exportBaseDir() const
{
    // Carpeta base: directorio de ejecución actual (en Qt Creator se configura en Run settings)
    QDir dir(QDir::currentPath());

    // Subcarpeta del proyecto para exports
    const QString sub = "ddl_exports";
    if (!dir.exists(sub)) {
        dir.mkpath(sub);
    }

    return dir.filePath(sub);
}

QString MainWindow::ddlForItem(QTreeWidgetItem* it)
{
    if (!it) return {};

    const QString t = typeOf(it);

    if (t == "table") {
        return m_meta.showCreateTable(dbOf(it), nameOf(it));
    }

    if (t == "db") {
        // MariaDB soporta CREATE DATABASE
        QString db = nameOf(it);
        db.replace("`", "");
        return QString("CREATE DATABASE `%1`;").arg(db);
    }

    return {};
}

QString MainWindow::suggestedDdlFileNameForItem(QTreeWidgetItem* it) const
{
    if (!it) return "ddl.sql";

    const QString t = typeOf(it);

    if (t == "table") {
        return QString("%1_%2.sql").arg(dbOf(it), nameOf(it));
    }

    if (t == "db") {
        return QString("%1.sql").arg(nameOf(it));
    }

    return "ddl.sql";
}


QString MainWindow::exportFilePathForItem(QTreeWidgetItem* it) const
{
    QDir dir(exportBaseDir());
    QString fileName = suggestedDdlFileNameForItem(it);
    if (fileName.trimmed().isEmpty()) fileName = "ddl.sql";

    QString full = dir.filePath(fileName);
    if (!QFile::exists(full)) return full;

    const QString ts = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    const int dot = fileName.lastIndexOf('.');
    if (dot > 0) {
        const QString n = fileName.left(dot);
        const QString ext = fileName.mid(dot);
        return dir.filePath(QString("%1_%2%3").arg(n, ts, ext));
    }
    return dir.filePath(QString("%1_%2").arg(fileName, ts));
}
