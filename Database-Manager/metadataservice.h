#pragma once
#include <QStringList>

class DbSession;

class MetadataService {
public:
    explicit MetadataService(DbSession* s);
    QStringList listDatabases();
    QStringList listTables(const QString& db);
    QString showCreateTable(const QString& db,const QString& t);
private:
    DbSession* s;
};
