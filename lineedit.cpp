

#include "lineedit.h"

LineEdit::LineEdit(QWidget *parent) : Editor(parent) {
    edit = new QLineEdit; 
    layout = new QHBoxLayout;
    layout->addWidget(edit);
    setLayout(layout);
}

void LineEdit::setMargins(int top, int right, int bottom, int left){
    layout->setContentsMargins(top,right,bottom,left);
}

void LineEdit::enterEditMode(){
    edit->setDisabled(true);
}

void LineEdit::exitEditMode(){
    edit->setDisabled(false);
    edit->setCursor(Qt::IBeamCursor);
}

void LineEdit::setActiveCursor(Qt::CursorShape shape){
    edit->setCursor(shape);
}

bool LineEdit::allowResizeWidth(){
    return true;
}

bool LineEdit::allowResizeHeight(){
    return false;
}

void LineEdit::paintEvent(QPaintEvent *event){
    QPainter painter(this);
}

QSize LineEdit::sizeHint() {
    return QSize(200, 35);
}

