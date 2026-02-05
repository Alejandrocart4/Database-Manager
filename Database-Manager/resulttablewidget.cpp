#include "ResultTableWidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlDatabase>

ResultTableWidget::ResultTableWidget(QWidget* p):QWidget(p){
    view=new QTableView;
    model=new QSqlQueryModel(this);
    view->setModel(model);
    auto* l=new QVBoxLayout(this);
    l->addWidget(view);
}

void ResultTableWidget::setQuery(const QString& sql,const QString& c){
    model->setQuery(sql,QSqlDatabase::database(c));
}
