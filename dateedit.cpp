
#include "dateedit.h"

DateEdit::DateEdit(const QString& label, QWidget *parent) : TemplateWidget(parent) {
    setLabel(label);
    edit = new QDateTimeEdit;
    edit->setCalendarPopup(true);
    addWidget(edit);
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

int DateEdit::getLeftAlignmentHint(){
    return edit->x()+2;
}

int DateEdit::getTopAlignmentHint(){
    return edit->y();;
}

bool DateEdit::allowResizeWidth(){
    return false;
}

bool DateEdit::allowResizeHeight(){
    return false;
}

