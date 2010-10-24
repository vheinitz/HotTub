

 #include <Qt>
 #include <QtGui>

 #include "templatewidget.h"

 TemplateWidget::TemplateWidget(QWidget *parent)
     : QWidget(parent)
 {
	layout = new QHBoxLayout;
	label = new QLabel;
	
     layout->addWidget(label, Qt::AlignTop);
	
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
    setActiveCursor(shape);
    
}

