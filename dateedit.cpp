
#include "dateedit.h"
#include "qcouch/document.h"

DateEdit::DateEdit(QWidget *parent) : Editor(parent) {
    edit = new QDateTimeEdit;
    edit->setCalendarPopup(true);
    layout = new QHBoxLayout;
    layout->addWidget(edit);
    setLayout(layout);
}

void DateEdit::setMargins(int top, int right, int bottom, int left){
    layout->setContentsMargins(top,right,bottom,left);
}

void DateEdit::enterEditMode(){
    edit->setDisabled(true);
}

void DateEdit::exitEditMode(){
    edit->setDisabled(false);
}

void DateEdit::setActiveCursor(Qt::CursorShape shape){
    edit->setCursor(shape);
}

bool DateEdit::allowResizeWidth(){
    return false;
}

bool DateEdit::allowResizeHeight(){
    return false;
}

QSize DateEdit::sizeHint(){
    return QSize(120, edit->sizeHint().height());
}   

int DateEdit::getLeftAlignmentHint(){
    return edit->x();
}

void DateEdit::setField(const QString &){
    
}

void DateEdit::loadDocument(Document doc){
    
}

void DateEdit::saveChanges(Document& doc){
    
}


bool DateEdit::hasChanges(){
    return false;
}
