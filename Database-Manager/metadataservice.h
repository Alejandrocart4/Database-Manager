#pragma once
#include <QStringList>
#include <QString>

class DbSession;

class MetadataService {
public:
    explicit MetadataService(DbSession* s);

    QStringList listDatabases();
    QStringList listTables(const QString& db);

    QStringList listViews(const QString& db);
    QStringList listTriggers(const QString& db);
    QStringList listFunctions(const QString& db);
    QStringList listProcedures(const QString& db);

    QStringList listIndexes(const QString& db, const QString& table);

    QString showCreateTable(const QString& db, const QString& t);
    QString showCreateView(const QString& db, const QString& v);
    QString showCreateTrigger(const QString& db, const QString& tr);
    QString showCreateFunction(const QString& db, const QString& fn);
    QString showCreateProcedure(const QString& db, const QString& sp);

private:
    DbSession* s;
};
