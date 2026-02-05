#pragma once
#include <QWidget>

class QPlainTextEdit;
class QPushButton;

class SqlConsoleWidget : public QWidget {
    Q_OBJECT
public:
    explicit SqlConsoleWidget(QWidget* parent=nullptr);
signals:
    void executeRequested(const QString& sql);
private:
    QPlainTextEdit* edit;
    QPushButton* btn;
};
