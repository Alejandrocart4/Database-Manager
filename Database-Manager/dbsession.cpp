#include "DbSession.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

static bool looksLikeConnString(const QString& s)
{
    const QString u = s.toUpper();
    return u.contains("DRIVER=") || u.contains("SERVER=") || u.contains("UID=") ||
           u.contains("PWD=") || u.contains("DATABASE=") || u.contains("PORT=");
}

bool DbSession::openWithDsn(const QString& dsnOrConnStr, QString* err)
{
    if (QSqlDatabase::contains(conn)) {
        QSqlDatabase old = QSqlDatabase::database(conn);
        if (old.isValid()) old.close();
        QSqlDatabase::removeDatabase(conn);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", conn);

    if (looksLikeConnString(dsnOrConnStr)) {
        db.setDatabaseName(dsnOrConnStr);
    } else {
        db.setDatabaseName("DSN=" + dsnOrConnStr + ";");
    }

    if (!db.open()) {
        if (err) *err = db.lastError().text();
        return false;
    }
    return true;
}

QSqlDatabase DbSession::db() const
{
    return QSqlDatabase::database(conn);
}

QString DbSession::q(const QString& s)
{
    return "`" + s + "`";
}
