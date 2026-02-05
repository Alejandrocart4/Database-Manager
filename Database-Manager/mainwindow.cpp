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

    // Editor SQL + botón ejecutar (esto reemplaza el problema)
    m_sqlEdit = new QPlainTextEdit;
    m_sqlEdit->setPlaceholderText("Escribe SQL aquí y presiona Ejecutar...");
    m_btnExec = new QPushButton("Ejecutar");

    auto* execBar = new QWidget;
    auto* execLay = new QHBoxLayout(execBar);
    execLay->setContentsMargins(0,0,0,0);
    execLay->addWidget(m_btnExec);

    // Panel derecho: resultados arriba, DDL medio, SQL abajo
    auto* right = new QSplitter(Qt::Vertical);
    right->addWidget(m_results);
    right->addWidget(m_ddl);

    auto* sqlBlock = new QWidget;
    auto* sqlLay = new QVBoxLayout(sqlBlock);
    sqlLay->setContentsMargins(0,0,0,0);
    sqlLay->addWidget(execBar);
    sqlLay->addWidget(m_sqlEdit);

    right->addWidget(sqlBlock);

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
    connect(m_btnExec, &QPushButton::clicked, this, [this](){
        const QString sql = m_sqlEdit->toPlainText().trimmed();
        if(sql.isEmpty()) return;
        executeSql(sql);
    });
}

/*void MainWindow::buildUi()
{
    m_tree = new QTreeWidget;
    m_tree->setHeaderLabel("Servidor");

    m_results = new ResultTableWidget;
    m_ddl = new QPlainTextEdit;
    m_console = new SqlConsoleWidget;

    auto* right = new QSplitter(Qt::Vertical);
    right->addWidget(m_results);
    right->addWidget(m_ddl);

    auto* main = new QSplitter(Qt::Horizontal);
    main->addWidget(m_tree);
    main->addWidget(right);

    auto* root = new QWidget;
    auto* lay = new QVBoxLayout(root);
    lay->addWidget(main);
    lay->addWidget(m_console);

    setCentralWidget(root);

    connect(m_tree,&QTreeWidget::itemExpanded,this,[&](QTreeWidgetItem* i){
        if(typeOf(i)=="db" && i->childCount()==0)
            loadDbChildren(nameOf(i));
    });

    connect(m_tree,&QTreeWidget::itemSelectionChanged,this,&MainWindow::showDdlForNode);
    connect(m_console,&SqlConsoleWidget::executeRequested,this,&MainWindow::executeSql);
}*/

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

void MainWindow::showDdlForNode()
{
    auto* it = m_tree->currentItem();
    if(!it) return;

    if(typeOf(it)=="table")
        m_ddl->setPlainText(m_meta.showCreateTable(dbOf(it),nameOf(it)));
}

void MainWindow::executeSql(const QString& sql)
{
    m_results->setQuery(sql,"odbc_conn");
}
