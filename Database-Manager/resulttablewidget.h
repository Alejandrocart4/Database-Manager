#pragma once
#include <QWidget>

class QTableView;
class QSqlQueryModel;

class ResultTableWidget : public QWidget {
    Q_OBJECT
public:
    explicit ResultTableWidget(QWidget* parent=nullptr);
    void setQuery(const QString& sql,const QString& conn);
private:
    QTableView* view;
    QSqlQueryModel* model;
};
