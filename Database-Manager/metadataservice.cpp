#include "MetadataService.h"
#include "DbSession.h"
#include <QtSql/QSqlQuery>

MetadataService::MetadataService(DbSession* s):s(s){}

QStringList MetadataService::listDatabases(){
    QStringList r; QSqlQuery q(s->db());
    q.exec("SHOW DATABASES");
    while(q.next()) r<<q.value(0).toString();
    return r;
}

QStringList MetadataService::listTables(const QString& db){
    QStringList r; QSqlQuery q(s->db());
    q.exec("SHOW TABLES FROM "+DbSession::q(db));
    while(q.next()) r<<q.value(0).toString();
    return r;
}

QString MetadataService::showCreateTable(const QString& db,const QString& t){
    QSqlQuery q(s->db());
    q.exec("SHOW CREATE TABLE "+DbSession::q(db)+"."+DbSession::q(t));
    q.next();
    return q.value(1).toString()+";";
}
