
#include <Qt>
#include <QtGui>
#include <iostream>
#include "templatewidget.h"
#include "editor.h"


using namespace std;

TemplateWidget::TemplateWidget(Editor* edit, QWidget *parent)
     : QWidget(parent)
{
    layout = new QHBoxLayout;
    stack = new QStackedLayout;
	label = new QLabel;
    labelEdit = new QLineEdit;
    editor = edit;
    
    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->addWidget(label);
    labelLayout->addStretch();
    
    QWidget *labelContainer = new QWidget;
    labelLayout->setContentsMargins(0,MARGIN,0,0);
    labelContainer->setLayout(labelLayout);
    
    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->setContentsMargins(0,0,0,0);
    editLayout->addWidget(labelEdit);
    editLayout->addStretch();
    
    QWidget *editContainer = new QWidget;
    editContainer->setLayout(editLayout);
    stack->setContentsMargins(0,0,0,0);
    stack->addWidget(labelContainer);
    stack->addWidget(editContainer);

    
    QWidget *widget = new QWidget;
    widget->setMaximumWidth(100);
    widget->setMinimumWidth(100);
    widget->setLayout(stack);
    
    layout->addWidget(widget);
    edit->setMargins(0, 0, 0, 0);
    layout->addWidget(edit);
    layout->setContentsMargins(5,MARGIN,5,0);
    QWidget::setLayout(layout);
    
    
    connect(labelEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateLabel(const QString&)));
    
}

QSize TemplateWidget::sizeHint(){
    QSize editorHint = editor->sizeHint();
    return QSize(editorHint.width()+150, editorHint.height()+MARGIN);
}

int TemplateWidget::getLeftAlignmentHint(){
    return editor->x() + editor->getLeftAlignmentHint();
}

int TemplateWidget::getTopAlignmentHint(){
    return editor->y();
}

void TemplateWidget::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    //painter.fillRect(0, 0, width()-1, height()-1, Qt::red);
    //painter.drawRect(0,0,width()-1,height()-1);
    QWidget::paintEvent(event);
}

void TemplateWidget::updateLabel(const QString& text){
    setLabel(text);
}

void TemplateWidget::setLabel(const QString& text){
    label->setText(text);
    labelEdit->setText(text);
}

void TemplateWidget::setCursor(Qt::CursorShape shape){
    QWidget::setCursor(shape);
    label->setCursor(shape);
    editor->setActiveCursor(shape);
}

void TemplateWidget::beginEditing(){
    setCursor(Qt::OpenHandCursor);
    stack->setCurrentIndex(1);
    editor->enterEditMode();
}

void TemplateWidget::stopEditing(){
    setCursor(Qt::ArrowCursor);
    stack->setCurrentIndex(0);
    editor->exitEditMode();
}

bool TemplateWidget::allowResizeWidth(){
    return editor->allowResizeWidth();
}


bool TemplateWidget::allowResizeHeight() {
    return editor->allowResizeHeight();
}


