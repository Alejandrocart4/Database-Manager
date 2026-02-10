#include "SqlConsoleWidget.h"
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextCursor>

class SqlSyntaxHighlighter : public QSyntaxHighlighter {
public:
    explicit SqlSyntaxHighlighter(QTextDocument* parent)
        : QSyntaxHighlighter(parent)
    {
        QTextCharFormat kw;
        kw.setForeground(QColor("#569CD6"));
        kw.setFontWeight(QFont::Bold);

        const QStringList keywords = {
            "select","from","where","insert","into","values","update","set","delete",
            "create","table","view","drop","alter","database","show","describe","use",
            "join","left","right","inner","outer","on","group","by","order","having",
            "limit","as","distinct"
        };

        for (const auto& k : keywords) {
            rules.append({QRegularExpression("\\b" + k + "\\b", QRegularExpression::CaseInsensitiveOption), kw});
        }
    }

protected:
    void highlightBlock(const QString& text) override
    {
        for (const auto& r : rules) {
            auto it = r.re.globalMatch(text);
            while (it.hasNext()) {
                auto m = it.next();
                setFormat(m.capturedStart(), m.capturedLength(), r.fmt);
            }
        }
    }

private:
    struct Rule { QRegularExpression re; QTextCharFormat fmt; };
    QVector<Rule> rules;
};


SqlConsoleWidget::SqlConsoleWidget(QWidget* p):QWidget(p){
    edit = new QPlainTextEdit;
    edit->setPlaceholderText("Escribe SQL aquí...");

    new SqlSyntaxHighlighter(edit->document());

    btn = new QPushButton("Ejecutar");
    status = new QLabel;
    status->setWordWrap(true);
    status->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto* topBar = new QWidget;
    auto* topLay = new QHBoxLayout(topBar);
    topLay->setContentsMargins(0,0,0,0);
    topLay->addWidget(btn);

    auto* l = new QVBoxLayout(this);
    l->addWidget(topBar);
    l->addWidget(status);
    l->addWidget(edit, 1);

    connect(btn, &QPushButton::clicked, this, [this](){
        const QString s = edit->toPlainText().trimmed();
        if (s.isEmpty()) {
            setStatusError("SQL vacío.");
            return;
        }
        emit executeRequested(s);
    });
}

QString SqlConsoleWidget::sql() const { return edit->toPlainText(); }

void SqlConsoleWidget::setSql(const QString& s){
    edit->setPlainText(s);
    edit->moveCursor(QTextCursor::End);
}

void SqlConsoleWidget::setStatusOk(const QString& message){
    status->setText(message);
    status->setStyleSheet("color: #9CDCFE;");
    edit->setStyleSheet("border: 1px solid #2D2D2D; border-radius: 4px;");
}

void SqlConsoleWidget::setStatusError(const QString& message){
    status->setText(message);
    status->setStyleSheet("color: #F44747;");
    edit->setStyleSheet("border: 1px solid #F44747; border-radius: 4px;");
}
