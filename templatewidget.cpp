
#include <Qt>
#include <QtGui>
#include <iostream>
#include "templatewidget.h"

using namespace std;

TemplateWidget::TemplateWidget(QWidget *parent)
     : QWidget(parent)
{
    layout = new QHBoxLayout;
    stack = new QStackedLayout;
	label = new QLabel;
    labelEdit = new QLineEdit;
	
    QVBoxLayout *labelLayout = new QVBoxLayout;
    label->setAlignment(Qt::AlignRight);
    labelLayout->addWidget(label);
    labelLayout->addStretch();
    
    QWidget *labelContainer = new QWidget;
    labelContainer->setLayout(labelLayout);
    
    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->addWidget(labelEdit);
    editLayout->addStretch();
    
    QWidget *editContainer = new QWidget;
    editContainer->setLayout(editLayout);
    
    stack->addWidget(labelContainer);
    stack->addWidget(editContainer);
    
    QWidget *widget = new QWidget;
    widget->setLayout(stack);
    widget->setMinimumWidth(100);
    widget->setMaximumWidth(250);
    
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(widget);
	QWidget::setLayout(layout);
    
    setMinimumSize(200, 50);
    setContentsMargins(0,0,0,0);
    connect(labelEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateLabel(const QString&)));
    
}

void TemplateWidget::updateLabel(const QString& text){
    setLabel(text);
}

void TemplateWidget::setLabel(const QString& text){
    label->setText(text);
    labelEdit->setText(text);
}

void TemplateWidget::setLayout(QLayout *l){
    layout->addLayout(l);
}

void TemplateWidget::addWidget(QWidget *widget){
    layout->addWidget(widget);
}
 
void TemplateWidget::setCursor(Qt::CursorShape shape){
    QWidget::setCursor(shape);
    label->setCursor(shape);
    setActiveCursor(shape);
}

void TemplateWidget::beginEditing(){
    setCursor(Qt::OpenHandCursor);
    stack->setCurrentIndex(1);
    enterEditMode();
}

void TemplateWidget::stopEditing(){
    setCursor(Qt::ArrowCursor);
    stack->setCurrentIndex(0);
    exitEditMode();
}