#include "ResultTableWidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

static bool looksLikeResultSet(const QString& sql)
{
    const QString s = sql.trimmed();
    if (s.isEmpty()) return false;

    const QString u = s.left(32).toUpper();
    return u.startsWith("SELECT") || u.startsWith("SHOW") || u.startsWith("DESCRIBE") ||
           u.startsWith("DESC") || u.startsWith("EXPLAIN") || u.startsWith("WITH");
}

ResultTableWidget::ResultTableWidget(QWidget* p):QWidget(p){
    view=new QTableView;
    model=new QSqlQueryModel(this);
    view->setModel(model);

    info = new QLabel;
    info->setWordWrap(true);
    info->setTextInteractionFlags(Qt::TextSelectableByMouse);

    stack = new QStackedWidget;
    stack->addWidget(view);  // index 0
    stack->addWidget(info);  // index 1

    auto* l=new QVBoxLayout(this);
    l->addWidget(stack);
}

bool ResultTableWidget::setQuery(const QString& sql, const QString& c, QString* outError)
{
    if (outError) outError->clear();

    QSqlDatabase db = QSqlDatabase::database(c);
    if (!db.isValid() || !db.isOpen()) {
        const QString e = "Conexión inválida o cerrada.";
        if (outError) *outError = e;
        info->setText(e);
        stack->setCurrentWidget(info);
        return false;
    }

    const QString s = sql.trimmed();
    if (s.isEmpty()) {
        info->setText("SQL vacío.");
        stack->setCurrentWidget(info);
        return false;
    }

    if (looksLikeResultSet(s)) {
        model->setQuery(s, db);
        const auto err = model->lastError();
        if (err.isValid()) {
            const QString e = err.text();
            if (outError) *outError = e;
            info->setText(e);
            stack->setCurrentWidget(info);
            return false;
        }
        stack->setCurrentWidget(view);
        return true;
    }

    // DDL / DML sin result set.
    QSqlQuery q(db);
    const bool ok = q.exec(s);
    if (!ok) {
        const QString e = q.lastError().text();
        if (outError) *outError = e;
        info->setText(e);
        stack->setCurrentWidget(info);
        return false;
    }

    const qint64 aff = q.numRowsAffected();
    const QString msg = (aff >= 0)
                            ? QString("Ejecutado correctamente. Filas afectadas: %1").arg(aff)
                            : QString("Ejecutado correctamente.");
    info->setText(msg);
    stack->setCurrentWidget(info);
    return true;
}
