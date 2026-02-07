#include "SqlConsoleWidget.h"
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextCursor>

namespace {

class SqlSyntaxHighlighter final : public QSyntaxHighlighter
{
public:
    explicit SqlSyntaxHighlighter(QTextDocument* doc) : QSyntaxHighlighter(doc)
    {
        QTextCharFormat kw;   kw.setForeground(QColor("#4FC1FF")); kw.setFontWeight(QFont::Bold);
        QTextCharFormat fn;   fn.setForeground(QColor("#C586C0"));
        QTextCharFormat num;  num.setForeground(QColor("#B5CEA8"));
        QTextCharFormat str;  str.setForeground(QColor("#CE9178"));
        QTextCharFormat cm;   cm.setForeground(QColor("#6A9955"));
        QTextCharFormat op;   op.setForeground(QColor("#D4D4D4"));

        auto add = [&](const QString& pattern, const QTextCharFormat& f){
            m_rules.push_back({QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption), f});
        };

        // Keywords (subset)
        add(R"(\b(SELECT|FROM|WHERE|JOIN|LEFT|RIGHT|INNER|OUTER|FULL|ON|AS|AND|OR|NOT|IN|IS|NULL|LIKE|BETWEEN|GROUP\s+BY|ORDER\s+BY|HAVING|LIMIT|OFFSET|UNION|ALL|DISTINCT|INSERT|INTO|VALUES|UPDATE|SET|DELETE|CREATE|ALTER|DROP|TRUNCATE|RENAME|TABLE|DATABASE|VIEW|INDEX|PRIMARY\s+KEY|FOREIGN\s+KEY|REFERENCES|CONSTRAINT|DEFAULT|AUTO_INCREMENT|ENGINE|CHARSET|COLLATE|USE|SHOW|DESCRIBE|EXPLAIN|WITH)\b)", kw);

        // Common functions
        add(R"(\b(COUNT|SUM|AVG|MIN|MAX|NOW|CURRENT_DATE|CURRENT_TIMESTAMP|COALESCE|IFNULL|NULLIF|CAST|CONVERT|DATE|DATEDIFF|TIMESTAMPDIFF|ROUND|FLOOR|CEIL|LOWER|UPPER|SUBSTRING|CONCAT)\b)", fn);

        // Numbers
        add(R"(\b\d+(?:\.\d+)?\b)", num);

        // Strings '...'
        add(R"('([^'\\]|\\.)*')", str);
        // Identifiers with backticks
        add(R"(`[^`]*`)", str);

        // Operators / punctuation
        add(R"([=<>!\+\-\*/%\.,\(\);])", op);

        m_lineComment = QRegularExpression(R"(--[^\n]*)");
        m_blockCommentStart = QRegularExpression(R"(/\*)");
        m_blockCommentEnd = QRegularExpression(R"(\*/)");
        m_commentFormat = cm;
    }

protected:
    void highlightBlock(const QString& text) override
    {
        // Apply simple rules
        for (const auto& r : m_rules) {
            auto it = r.re.globalMatch(text);
            while (it.hasNext()) {
                const auto m = it.next();
                setFormat(m.capturedStart(), m.capturedLength(), r.fmt);
            }
        }

        // Line comment
        {
            auto it = m_lineComment.globalMatch(text);
            while (it.hasNext()) {
                const auto m = it.next();
                setFormat(m.capturedStart(), m.capturedLength(), m_commentFormat);
            }
        }

        // Block comments (/* ... */) across blocks
        setCurrentBlockState(0);
        int start = 0;
        if (previousBlockState() != 1)
            start = text.indexOf(m_blockCommentStart);
        else
            start = 0;

        while (start >= 0) {
            int end = text.indexOf(m_blockCommentEnd, start);
            int len;
            if (end == -1) {
                setCurrentBlockState(1);
                len = text.length() - start;
            } else {
                len = end - start + 2;
            }
            setFormat(start, len, m_commentFormat);
            start = text.indexOf(m_blockCommentStart, start + len);
        }
    }

private:
    struct Rule { QRegularExpression re; QTextCharFormat fmt; };
    QVector<Rule> m_rules;

    QRegularExpression m_lineComment;
    QRegularExpression m_blockCommentStart;
    QRegularExpression m_blockCommentEnd;
    QTextCharFormat m_commentFormat;
};

}

SqlConsoleWidget::SqlConsoleWidget(QWidget* p):QWidget(p){
    edit = new QPlainTextEdit;
    edit->setPlaceholderText("Escribe SQL aquí...");

    // Syntax highlighting (SQL)
    new SqlSyntaxHighlighter(edit->document());

    btn = new QPushButton("Ejecutar");
    status = new QLabel;
    status->setWordWrap(true);
    status->setTextInteractionFlags(Qt::TextSelectableByMouse);

    history = new QListWidget;
    history->setMaximumHeight(120);
    history->setSelectionMode(QAbstractItemView::SingleSelection);

    auto* topBar = new QWidget;
    auto* topLay = new QHBoxLayout(topBar);
    topLay->setContentsMargins(0,0,0,0);
    topLay->addWidget(btn);

    auto* l = new QVBoxLayout(this);
    l->addWidget(topBar);
    l->addWidget(status);
    l->addWidget(edit, 1);
    l->addWidget(history);

    connect(btn, &QPushButton::clicked, this, [this](){
        const QString s = edit->toPlainText().trimmed();
        if (s.isEmpty()) {
            setStatusError("SQL vacío.");
            return;
        }
        addToHistory(s);
        emit executeRequested(s);
    });

    connect(history, &QListWidget::itemClicked, this, [this](QListWidgetItem* it){
        if (!it) return;
        setSql(it->text());
    });
}

QString SqlConsoleWidget::sql() const
{
    return edit->toPlainText();
}

void SqlConsoleWidget::setSql(const QString& s)
{
    edit->setPlainText(s);
    edit->moveCursor(QTextCursor::End);
}

void SqlConsoleWidget::setStatusOk(const QString& message)
{
    status->setText(message);
    status->setStyleSheet("color: #9CDCFE;");
    edit->setStyleSheet("border: 1px solid #2D2D2D; border-radius: 4px;");
}

void SqlConsoleWidget::setStatusError(const QString& message)
{
    status->setText(message);
    status->setStyleSheet("color: #F44747;");
    edit->setStyleSheet("border: 1px solid #F44747; border-radius: 4px;");
}

void SqlConsoleWidget::addToHistory(const QString& s)
{
    const QString sqlTrim = s.trimmed();
    if (sqlTrim.isEmpty()) return;

    // Evita duplicar si es la misma que la última.
    if (history->count() > 0) {
        const QString last = history->item(0)->text();
        if (last == sqlTrim) return;
    }

    history->insertItem(0, sqlTrim);
    history->setCurrentRow(0);

    constexpr int kMax = 100;
    while (history->count() > kMax)
        delete history->takeItem(history->count() - 1);
}
