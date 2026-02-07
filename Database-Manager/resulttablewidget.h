#pragma once
#include <QWidget>

class QTableView;
class QSqlQueryModel;
class QLabel;
class QStackedWidget;

class ResultTableWidget : public QWidget {
    Q_OBJECT
public:
    explicit ResultTableWidget(QWidget* parent=nullptr);

    bool setQuery(const QString& sql, const QString& conn, QString* outError = nullptr);

private:
    QTableView* view;
    QSqlQueryModel* model;
    QLabel* info;
    QStackedWidget* stack;
};
