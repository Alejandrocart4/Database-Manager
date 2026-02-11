#include "MetadataService.h"
#include "DbSession.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QSet>

MetadataService::MetadataService(DbSession* s):s(s){}

QStringList MetadataService::listDatabases(){
    QStringList r; QSqlQuery q(s->db());
    q.exec("SHOW DATABASES");
    while(q.next()) r << q.value(0).toString();
    return r;
}

QStringList MetadataService::listTables(const QString& db){
    // IMPORTANTE: SHOW TABLES incluye vistas. Para que “Tablas” sea solo BASE TABLE:
    QStringList r; QSqlQuery q(s->db());
    q.exec("SHOW FULL TABLES FROM " + DbSession::q(db) + " WHERE Table_type = 'BASE TABLE'");
    while(q.next()) r << q.value(0).toString();
    return r;
}

QStringList MetadataService::listViews(const QString& db)
{
    QStringList r; QSqlQuery q(s->db());
    q.exec("SHOW FULL TABLES FROM " + DbSession::q(db) + " WHERE Table_type = 'VIEW'");
    while (q.next()) r << q.value(0).toString();
    return r;
}

QStringList MetadataService::listTriggers(const QString& db)
{
    QStringList r; QSqlQuery q(s->db());
    q.exec("SHOW TRIGGERS FROM " + DbSession::q(db));
    while (q.next()) r << q.value(0).toString(); // columna Trigger
    return r;
}

QStringList MetadataService::listFunctions(const QString& db)
{
    QStringList r; QSqlQuery q(s->db());
    q.prepare("SHOW FUNCTION STATUS WHERE Db = ?");
    q.addBindValue(db);
    if (!q.exec()) return r;

    const int nameIdx = q.record().indexOf("Name");
    while (q.next()) {
        r << (nameIdx >= 0 ? q.value(nameIdx).toString() : q.value(1).toString());
    }
    return r;
}

QStringList MetadataService::listProcedures(const QString& db)
{
    QStringList r; QSqlQuery q(s->db());
    q.prepare("SHOW PROCEDURE STATUS WHERE Db = ?");
    q.addBindValue(db);
    if (!q.exec()) return r;

    const int nameIdx = q.record().indexOf("Name");
    while (q.next()) {
        r << (nameIdx >= 0 ? q.value(nameIdx).toString() : q.value(1).toString());
    }
    return r;
}

QStringList MetadataService::listIndexes(const QString& db, const QString& table)
{
    // MariaDB: forma correcta (NO usar db.table aquí)
    QSet<QString> uniq;
    QSqlQuery q(s->db());
    q.exec("SHOW INDEX FROM " + DbSession::q(table) + " FROM " + DbSession::q(db));

    const int keyIdx = q.record().indexOf("Key_name");
    while (q.next()) {
        const QString k = (keyIdx >= 0 ? q.value(keyIdx).toString() : q.value(2).toString());
        if (!k.isEmpty()) uniq.insert(k);
    }

    QStringList r = uniq.values();
    r.sort(Qt::CaseInsensitive);
    return r;
}

QString MetadataService::showCreateTable(const QString& db,const QString& t){
    QSqlQuery q(s->db());
    q.exec("SHOW CREATE TABLE " + DbSession::q(db) + "." + DbSession::q(t));
    if (!q.next()) return {};
    return q.value(1).toString() + ";";
}

QString MetadataService::showCreateView(const QString& db, const QString& v)
{
    QSqlQuery q(s->db());
    q.exec("SHOW CREATE VIEW " + DbSession::q(db) + "." + DbSession::q(v));
    if (!q.next()) return {};
    return q.value(1).toString() + ";";
}

QString MetadataService::showCreateTrigger(const QString& db, const QString& tr)
{
    QSqlQuery q(s->db());
    q.exec("SHOW CREATE TRIGGER " + DbSession::q(db) + "." + DbSession::q(tr));
    if (!q.next()) return {};

    int idx = q.record().indexOf("SQL Original Statement");
    if (idx < 0) idx = q.record().indexOf("Create Trigger");
    return (idx >= 0 ? q.value(idx).toString() : q.value(2).toString()) + ";";
}

QString MetadataService::showCreateFunction(const QString& db, const QString& fn)
{
    QSqlQuery q(s->db());
    q.exec("SHOW CREATE FUNCTION " + DbSession::q(db) + "." + DbSession::q(fn));
    if (!q.next()) return {};

    int idx = q.record().indexOf("Create Function");
    if (idx < 0) idx = 2;
    return q.value(idx).toString() + ";";
}

QString MetadataService::showCreateProcedure(const QString& db, const QString& sp)
{
    QSqlQuery q(s->db());
    q.exec("SHOW CREATE PROCEDURE " + DbSession::q(db) + "." + DbSession::q(sp));
    if (!q.next()) return {};

    int idx = q.record().indexOf("Create Procedure");
    if (idx < 0) idx = 2;
    return q.value(idx).toString() + ";";
}
