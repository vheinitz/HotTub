
#include "combo.h"
#include <QtGui>

Combo::Combo(QWidget *parent) : Editor(parent) {
    edit = new QComboBox;
    layout = new QVBoxLayout;
    layout->addSpacing(4);
    layout->addWidget(edit);
    setLayout(layout);
    
    edit->insertItem(0,"One");
    edit->insertItem(1,"Two");
    edit->insertItem(2,"Three");
}

void Combo::setMargins(int top, int right, int bottom, int left){
    layout->setContentsMargins(top,right,bottom,left);
}

void Combo::enterEditMode(){
    edit->setDisabled(true);
}

void Combo::exitEditMode(){
    edit->setDisabled(false);
    edit->setCursor(Qt::IBeamCursor);
}

void Combo::setActiveCursor(Qt::CursorShape shape){
    edit->setCursor(shape);    
}

bool Combo::allowResizeWidth(){
    return true;
}

bool Combo::allowResizeHeight(){
    return true;
}

QSize Combo::sizeHint(){
    return edit->sizeHint();
}

int Combo::getLeftAlignmentHint(){
    return edit->x();
}


QAction* Combo::configurationAction(QToolBar *toolbar){
    return new QAction(toolbar);
}

