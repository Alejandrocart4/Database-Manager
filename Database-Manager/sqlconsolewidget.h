#pragma once
#include <QWidget>

class QPlainTextEdit;
class QPushButton;
class QLabel;

class SqlConsoleWidget : public QWidget {
    Q_OBJECT
public:
    explicit SqlConsoleWidget(QWidget* parent=nullptr);

    QString sql() const;
    void setSql(const QString& sql);

    void setStatusOk(const QString& message);
    void setStatusError(const QString& message);

signals:
    void executeRequested(const QString& sql);

private:
    QPlainTextEdit* edit;
    QPushButton* btn;
    QLabel* status;
};
