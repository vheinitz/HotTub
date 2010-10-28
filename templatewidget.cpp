
#include <Qt>
#include <QtGui>

#include "templatewidget.h"

TemplateWidget::TemplateWidget(QWidget *parent)
     : QWidget(parent)
{
    layout = new QHBoxLayout;
	label = new QLabel;
	/*layout->setHorizontalSpacing(8);
    //layout->addWidget(label, 0, 0, Qt::AlignTop);*/
	layout->setContentsMargins(0,0,0,0);
    layout->addWidget(label);
	QWidget::setLayout(layout);
    
}


void TemplateWidget::setLabel(const QString& text){
    label->setText(text);
}

void TemplateWidget::setLayout(QLayout *l){
    layout->addLayout(l);
    /*layout->addLayout(l, 0, 1, Qt::AlignTop);*/
}

void TemplateWidget::addWidget(QWidget *widget){
    layout->addWidget(widget);
    /*layout->addWidget(widget, 0, 1, Qt::AlignTop);*/
}
 
void TemplateWidget::setCursor(Qt::CursorShape shape){
    QWidget::setCursor(shape);
    setActiveCursor(shape);
}
