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
#include <QGuiApplication>
#include <QScreen>
#include <QCoreApplication>

static QString typeOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole).toString(); }
static QString dbOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole+1).toString(); }
static QString nameOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole+2).toString(); }
static QString tableOf(QTreeWidgetItem* i){ return i->data(0, Qt::UserRole+3).toString(); }

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_meta(&m_session)
{
    buildUi();

    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tree, &QWidget::customContextMenuRequested, this, [this](const QPoint& pos){
        QTreeWidgetItem* it = m_tree->itemAt(pos);
        if (!it) return;

        // Seleccionar item bajo el mouse para que las acciones operen sobre él
        m_tree->setCurrentItem(it);

        QMenu menu;

        // Root "MariaDB": toggle system schemas
        if (it == m_tree->topLevelItem(0)) {
            QAction* toggleSystem = menu.addAction(
                m_showSystemSchemas
                    ? "Ocultar bases del sistema (information_schema)"
                    : "Mostrar bases del sistema (information_schema)"
                );
            QAction* refreshAll = menu.addAction("Refrescar");

            QAction* chosen = menu.exec(m_tree->viewport()->mapToGlobal(pos));
            if (chosen == toggleSystem) {
                m_showSystemSchemas = !m_showSystemSchemas;
                loadDatabases();
            } else if (chosen == refreshAll) {
                loadDatabases();
            }
            return;
        }

        const QString t = typeOf(it);

        // Nodo de base de datos
        if (t == "db") {
            QAction* genDdl = menu.addAction("Generar DDL");
            QAction* expDdl = menu.addAction("Exportar DDL");
            menu.addSeparator();
            QAction* genAll = menu.addAction("Generar DDL General de la base de datos");
            QAction* expAll = menu.addAction("Exportar DDL General de la base de datos");
            menu.addSeparator();
            QAction* refreshDb = menu.addAction("Refrescar esta base");

            QAction* chosen = menu.exec(m_tree->viewport()->mapToGlobal(pos));
            if (!chosen) return;

            const QString dbName = nameOf(it);
            if (chosen == genDdl) {
                m_ddl->setPlainText(ddlForItem(it));
            } else if (chosen == expDdl) {
                exportDdlForItem(it);
            } else if (chosen == genAll) {
                m_ddl->setPlainText(ddlForDatabaseGeneral(dbName));
            } else if (chosen == expAll) {
                exportDdlGeneralForDatabase(dbName);
            } else if (chosen == refreshDb) {
                refreshDatabaseNode(dbName);
            }
            return;
        }

        // Nodos de objetos (tabla/vista/proc/func/trigger/index)
        const bool isObject = (t == "table" || t == "view" || t == "procedure" || t == "function" || t == "trigger" || t == "index");
        if (isObject) {
            QAction* genDdl = menu.addAction("Generar DDL");
            QAction* expDdl = menu.addAction("Exportar DDL");
            QAction* copyDdl = menu.addAction("Copiar DDL");

            QAction* chosen = menu.exec(m_tree->viewport()->mapToGlobal(pos));
            if (!chosen) return;

            if (chosen == genDdl) {
                m_ddl->setPlainText(ddlForItem(it));
            } else if (chosen == expDdl) {
                exportDdlForItem(it);
            } else if (chosen == copyDdl) {
                const QString ddl = ddlForItem(it);
                QApplication::clipboard()->setText(ddl);
                m_console->setStatusOk("DDL copiado");
            }
            return;
        }
    });

    LoginDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
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

    m_ready = true;

    resize(1200, 750);

    setMinimumSize(1100, 650);

    centerOnScreen();
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

    connect(m_tree, &QTreeWidget::itemExpanded, this, [&](QTreeWidgetItem* i){
        if(typeOf(i)=="db" && i->childCount()==0)
            loadDbChildren(nameOf(i));
        if(typeOf(i)=="table" && i->childCount()==0)
            loadTableChildren(i);
    });
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

    // TABLAS
    auto* tables = new QTreeWidgetItem(dbNode);
    tables->setText(0,"Tablas");

    for(const auto& t : m_meta.listTables(db)){
        auto* it = new QTreeWidgetItem(tables);
        it->setText(0,t);
        it->setData(0,Qt::UserRole,"table");
        it->setData(0,Qt::UserRole+1,db);
        it->setData(0,Qt::UserRole+2,t);

        // Para que se pueda expandir y cargar índices (flecha visible)
        it->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        new QTreeWidgetItem(it); // dummy child
    }


    auto* indexes = new QTreeWidgetItem(dbNode);
    indexes->setText(0, "Indices");

    const auto tablesList = m_meta.listTables(db);

    for (const auto& t : tablesList) {
        const auto idxs = m_meta.listIndexes(db, t);
        for (const auto& idx : idxs) {
            auto* it = new QTreeWidgetItem(indexes);
            it->setText(0, QString("%1.%2").arg(t, idx));
            it->setData(0, Qt::UserRole, "index");
            it->setData(0, Qt::UserRole+1, db);   // db
            it->setData(0, Qt::UserRole+2, idx);  // index name (PRIMARY, fk_..., etc.)
            it->setData(0, Qt::UserRole+3, t);    // table
        }
    }


    // VISTAS
    auto* views = new QTreeWidgetItem(dbNode);
    views->setText(0,"Vistas");
    for (const auto& v : m_meta.listViews(db)) {
        auto* it = new QTreeWidgetItem(views);
        it->setText(0, v);
        it->setData(0, Qt::UserRole, "view");
        it->setData(0, Qt::UserRole+1, db);
        it->setData(0, Qt::UserRole+2, v);
    }

    // FUNCIONES
    auto* funcs = new QTreeWidgetItem(dbNode);
    funcs->setText(0,"Funciones");
    for (const auto& fn : m_meta.listFunctions(db)) {
        auto* it = new QTreeWidgetItem(funcs);
        it->setText(0, fn);
        it->setData(0, Qt::UserRole, "function");
        it->setData(0, Qt::UserRole+1, db);
        it->setData(0, Qt::UserRole+2, fn);
    }

    // PROCEDURES
    auto* procs = new QTreeWidgetItem(dbNode);
    procs->setText(0,"Procedimientos");
    for (const auto& sp : m_meta.listProcedures(db)) {
        auto* it = new QTreeWidgetItem(procs);
        it->setText(0, sp);
        it->setData(0, Qt::UserRole, "procedure");
        it->setData(0, Qt::UserRole+1, db);
        it->setData(0, Qt::UserRole+2, sp);
    }

    // TRIGGERS
    auto* triggers = new QTreeWidgetItem(dbNode);
    triggers->setText(0,"Triggers");
    for (const auto& tr : m_meta.listTriggers(db)) {
        auto* it = new QTreeWidgetItem(triggers);
        it->setText(0, tr);
        it->setData(0, Qt::UserRole, "trigger");
        it->setData(0, Qt::UserRole+1, db);
        it->setData(0, Qt::UserRole+2, tr);
    }
}

void MainWindow::loadTableChildren(QTreeWidgetItem* tableNode)
{
    if (!tableNode || typeOf(tableNode) != "table") return;

    while (tableNode->childCount() > 0) {
        delete tableNode->takeChild(0);
    }

    const QString dbName = dbOf(tableNode);
    const QString tableName = nameOf(tableNode);

    auto* idxFolder = new QTreeWidgetItem(tableNode);
    idxFolder->setText(0, "Índices");

    const auto idxs = m_meta.listIndexes(dbName, tableName);
    for (const auto& idx : idxs) {
        auto* it = new QTreeWidgetItem(idxFolder);
        it->setText(0, idx);
        it->setData(0, Qt::UserRole, "index");
        it->setData(0, Qt::UserRole+1, dbName);
        it->setData(0, Qt::UserRole+2, idx);
        it->setData(0, Qt::UserRole+3, tableName);
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

    // Vuelve a cargar
    loadDbChildren(dbName);

    // Mantén expandido
    dbNode->setExpanded(true);
}

void MainWindow::showDdlForNode()
{
    auto* it = m_tree->currentItem();
    if (!it) return;

    const QString t = typeOf(it);


    if (t == "index") {
        const QString db = dbOf(it);
        const QString table = tableOf(it);
        const QString sql = QString("SHOW INDEX FROM %1 FROM %2;")
                                .arg(DbSession::q(table), DbSession::q(db));

        QString err;
        const bool ok = m_results->setQuery(sql, "odbc_conn", &err);
        if (!ok) m_console->setStatusError(err);
        else     m_console->setStatusOk("OK");

        m_ddl->setPlainText(sql + "\n-- Índice seleccionado: " + nameOf(it));
        return;
    }

    m_ddl->setPlainText(ddlForItem(it));
}

static QString firstTokenUpper(QString s)
{
    s = s.trimmed();
    if (s.isEmpty()) return {};

    while (s.startsWith("--")) {
        int nl = s.indexOf('\n');
        if (nl < 0) return {};
        s = s.mid(nl + 1).trimmed();
    }

    while (s.startsWith("/*")) {
        int end = s.indexOf("*/");
        if (end < 0) return {};
        s = s.mid(end + 2).trimmed();
    }

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
    // Intenta guardar dentro de la carpeta del proyecto.
    QDir dir(QDir::currentPath());
    QDir probe = dir;

    auto hasProjectMarker = [](const QDir& d) -> bool {
        if (d.exists("CMakeLists.txt")) return true;
        const QStringList pros = d.entryList(QStringList() << "*.pro", QDir::Files);
        return !pros.isEmpty();
    };

    for (int up = 0; up < 6; ++up) {
        if (hasProjectMarker(probe)) {
            dir = probe;
            break;
        }
        if (!probe.cdUp()) break;
    }

    const QString sub = "ddl_exports";
    if (!dir.exists(sub)) dir.mkpath(sub);
    return dir.filePath(sub);
}

static QString stripTrailingSemicolon(QString s)
{
    s = s.trimmed();
    if (s.endsWith(';')) s.chop(1);
    return s.trimmed();
}

static QString wrapWithDelimiter(const QString& ddl, const QString& delim)
{
    const QString body = stripTrailingSemicolon(ddl);
    if (body.isEmpty()) return {};

    QString out;
    out += QString("DELIMITER %1\n").arg(delim);
    out += body;
    out += QString("%1\n").arg(delim);
    out += "DELIMITER ;\n";
    return out;
}

QString MainWindow::ddlForItem(QTreeWidgetItem* it)
{
    if (!it) return {};

    const QString t = typeOf(it);

    if (t == "db") {
        QString db = nameOf(it);
        db.replace("`", "");
        return QString("CREATE DATABASE `%1`;").arg(db);
    }

    if (t == "table") {
        return m_meta.showCreateTable(dbOf(it), nameOf(it));
    }

    if (t == "view") {
        return m_meta.showCreateView(dbOf(it), nameOf(it));
    }

    if (t == "trigger") {
        return m_meta.showCreateTrigger(dbOf(it), nameOf(it));
    }

    if (t == "function") {
        return m_meta.showCreateFunction(dbOf(it), nameOf(it));
    }

    if (t == "procedure") {
        return m_meta.showCreateProcedure(dbOf(it), nameOf(it));
    }

    if (t == "index") {
        const QString db = dbOf(it);
        const QString table = tableOf(it);
        const QString idx = nameOf(it);
        return QString("SHOW INDEX FROM %1 FROM %2;\n-- Índice seleccionado: %3")
            .arg(DbSession::q(table), DbSession::q(db), idx);
    }

    return {};
}

QString MainWindow::ddlForDatabaseGeneral(const QString& dbName)
{
    if (dbName.trimmed().isEmpty()) return {};

    const QString db = dbName;
    QString out;
    out += "-- DDL General (export)\n";
    out += "-- Generado: " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "\n\n";
    out += QString("CREATE DATABASE IF NOT EXISTS %1;\n").arg(DbSession::q(db));
    out += QString("USE %1;\n\n").arg(DbSession::q(db));

    // Tablas (incluye índices dentro del CREATE TABLE)
    const auto tables = m_meta.listTables(db);
    for (const auto& t : tables) {
        out += "-- Tabla: " + t + "\n";
        out += m_meta.showCreateTable(db, t) + "\n\n";
    }

    // Vistas
    const auto views = m_meta.listViews(db);
    for (const auto& v : views) {
        out += "-- Vista: " + v + "\n";
        out += m_meta.showCreateView(db, v) + "\n\n";
    }

    // Rutinas y triggers requieren delimitador para ejecutarse como script
    const QString delim = "$$";

    const auto funcs = m_meta.listFunctions(db);
    for (const auto& fn : funcs) {
        out += "-- Función: " + fn + "\n";
        out += wrapWithDelimiter(m_meta.showCreateFunction(db, fn), delim) + "\n";
    }

    const auto procs = m_meta.listProcedures(db);
    for (const auto& sp : procs) {
        out += "-- Procedimiento: " + sp + "\n";
        out += wrapWithDelimiter(m_meta.showCreateProcedure(db, sp), delim) + "\n";
    }

    const auto triggers = m_meta.listTriggers(db);
    for (const auto& tr : triggers) {
        out += "-- Trigger: " + tr + "\n";
        out += wrapWithDelimiter(m_meta.showCreateTrigger(db, tr), delim) + "\n";
    }

    return out;
}

QString MainWindow::suggestedDdlFileNameForItem(QTreeWidgetItem* it) const
{
    if (!it) return "ddl.sql";

    const QString t = typeOf(it);

    if (t == "db")        return QString("%1_database.sql").arg(nameOf(it));
    if (t == "table")     return QString("%1_table_%2.sql").arg(dbOf(it), nameOf(it));
    if (t == "view")      return QString("%1_view_%2.sql").arg(dbOf(it), nameOf(it));
    if (t == "trigger")   return QString("%1_trigger_%2.sql").arg(dbOf(it), nameOf(it));
    if (t == "function")  return QString("%1_function_%2.sql").arg(dbOf(it), nameOf(it));
    if (t == "procedure") return QString("%1_procedure_%2.sql").arg(dbOf(it), nameOf(it));
    if (t == "index")     return QString("%1_index_%2_%3.sql").arg(dbOf(it), tableOf(it), nameOf(it));

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

QString MainWindow::exportFilePathForDatabaseGeneral(const QString& dbName) const
{
    QDir dir(exportBaseDir());
    QString fileName = QString("%1_ddl_general.sql").arg(dbName);
    fileName.replace('`', "");
    fileName.replace(' ', "_");

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

void MainWindow::exportDdlForItem(QTreeWidgetItem* it)
{
    if (!it) return;

    const QString ddl = ddlForItem(it);
    if (ddl.trimmed().isEmpty()) {
        QMessageBox::warning(this, "DDL", "No se pudo generar DDL para el nodo seleccionado.");
        return;
    }

    const QString path = exportFilePathForItem(it);

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Exportar DDL", "No se pudo escribir el archivo:\n" + path);
        return;
    }

    QTextStream ts(&f);
    ts << "-- Exportado por Database Manager\n";
    ts << "-- Fecha: " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n\n";
    ts << ddl << "\n";
    f.close();

    m_console->setStatusOk("Exportado: " + path);
}

void MainWindow::exportDdlGeneralForDatabase(const QString& dbName)
{
    const QString ddl = ddlForDatabaseGeneral(dbName);
    if (ddl.trimmed().isEmpty()) {
        QMessageBox::warning(this, "DDL General", "No se pudo generar el DDL general para la base seleccionada.");
        return;
    }

    const QString path = exportFilePathForDatabaseGeneral(dbName);
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Exportar DDL General", "No se pudo escribir el archivo:\n" + path);
        return;
    }

    QTextStream ts(&f);
    ts << ddl;
    f.close();

    m_console->setStatusOk("Exportado: " + path);
}

void MainWindow::centerOnScreen()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    if (!screen) return;

    const QRect avail = screen->availableGeometry();
    const QPoint center = avail.center();

    this->adjustSize();
    const QRect r = frameGeometry();
    move(center.x() - r.width() / 2, center.y() - r.height() / 2);
}
