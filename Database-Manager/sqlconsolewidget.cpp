#include "SqlConsoleWidget.h"
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

SqlConsoleWidget::SqlConsoleWidget(QWidget* p):QWidget(p){
    edit=new QPlainTextEdit;
    btn=new QPushButton("Ejecutar");
    auto* l=new QVBoxLayout(this);
    l->addWidget(btn);
    l->addWidget(edit);
    connect(btn,&QPushButton::clicked,this,[&](){
        emit executeRequested(edit->toPlainText());
    });
}
