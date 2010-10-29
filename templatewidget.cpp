
#include <Qt>
#include <QtGui>

#include "templatewidget.h"

TemplateWidget::TemplateWidget(QWidget *parent)
     : QWidget(parent)
{
    layout = new QHBoxLayout;
	label = new QLabel;
	layout->setContentsMargins(0,0,0,0);
    layout->addWidget(label);
	QWidget::setLayout(layout);
    
}


void TemplateWidget::setLabel(const QString& text){
    label->setText(text);
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
    enterEditMode();
}

void TemplateWidget::stopEditing(){
    setCursor(Qt::ArrowCursor);
    exitEditMode();
}