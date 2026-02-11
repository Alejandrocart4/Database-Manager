#include <QApplication>
#include <QPalette>
#include <QColor>
#include "MainWindow.h"

static void applyDarkTheme(QApplication& app)
{
    QPalette p;
    p.setColor(QPalette::Window, QColor("#1E1E1E"));
    p.setColor(QPalette::WindowText, QColor("#D4D4D4"));
    p.setColor(QPalette::Base, QColor("#252526"));
    p.setColor(QPalette::AlternateBase, QColor("#1E1E1E"));
    p.setColor(QPalette::ToolTipBase, QColor("#252526"));
    p.setColor(QPalette::ToolTipText, QColor("#D4D4D4"));
    p.setColor(QPalette::Text, QColor("#D4D4D4"));
    p.setColor(QPalette::Button, QColor("#2D2D30"));
    p.setColor(QPalette::ButtonText, QColor("#D4D4D4"));
    p.setColor(QPalette::BrightText, QColor("#F44747"));
    p.setColor(QPalette::Highlight, QColor("#264F78"));
    p.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));
    app.setPalette(p);

    // Controles principales
    app.setStyleSheet(
        "QWidget { background: #1E1E1E; color: #D4D4D4; }"
        "QTreeWidget, QPlainTextEdit, QTextEdit, QTableView, QListWidget {"
        "  background: #252526; border: 1px solid #2D2D2D; selection-background-color: #264F78;"
        "}"
        "QHeaderView::section { background: #2D2D30; border: 1px solid #2D2D2D; padding: 4px; }"
        "QPushButton { background: #0E639C; border: 1px solid #2D2D2D; padding: 6px 10px; border-radius: 4px; }"
        "QPushButton:hover { background: #1177BB; }"
        "QPushButton:pressed { background: #0B4F7A; }"
        "QSplitter::handle { background: #2D2D2D; }"
        );
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    applyDarkTheme(a);

    MainWindow w;

    if (!w.isReady()) {
        return 0;
    }

    w.show();

    return a.exec();
}
