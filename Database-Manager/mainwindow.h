// mainwindow.h

#pragma once
#include <QMainWindow>
#include "DbSession.h"
#include "MetadataService.h"

class QTreeWidgetItem;
class QTreeWidget;
class QPlainTextEdit;
class ResultTableWidget;
class SqlConsoleWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
        bool isReady() const { return m_ready; }

private:
    void buildUi();
    void loadDatabases();
    void loadDbChildren(const QString& dbName);
    void loadTableChildren(QTreeWidgetItem* tableNode);

    bool m_ready = false;
    void centerOnScreen();

    void showDdlForNode();
    void executeSql(const QString& sql);
    void refreshDatabaseNode(const QString& dbName);

    QString ddlForItem(QTreeWidgetItem* it);
    QString suggestedDdlFileNameForItem(QTreeWidgetItem* it) const;

    QString exportBaseDir() const;
    QString exportFilePathForItem(QTreeWidgetItem* it) const;

private:
    DbSession m_session;
    MetadataService m_meta;

    QTreeWidget* m_tree = nullptr;
    ResultTableWidget* m_results = nullptr;
    QPlainTextEdit* m_ddl = nullptr;
    SqlConsoleWidget* m_console = nullptr;

    bool m_showSystemSchemas = false;
};
