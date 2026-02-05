#pragma once
#include <QMainWindow>
#include "DbSession.h"
#include "MetadataService.h"

class QTreeWidget;
class QPlainTextEdit;
class ResultTableWidget;
class SqlConsoleWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void buildUi();
    void loadDatabases();
    void loadDbChildren(const QString& dbName);
    void showDdlForNode();
    void executeSql(const QString& sql);

private:
    DbSession m_session;
    MetadataService m_meta;

    QTreeWidget* m_tree = nullptr;
    ResultTableWidget* m_results = nullptr;
    QPlainTextEdit* m_ddl = nullptr;
    SqlConsoleWidget* m_console = nullptr;

    QString m_dsnName = "MariaDBLocal"; // TU DSN REAL
};
