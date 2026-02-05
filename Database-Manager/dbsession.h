#pragma once
#include <QSqlDatabase>
#include <QString>

class DbSession {
public:
    bool openWithDsn(const QString& dsnOrConnStr, QString* err = nullptr);
    QSqlDatabase db() const;
    static QString q(const QString& s);

private:
    QString conn = "odbc_conn";
};
