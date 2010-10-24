

#include "lineedit.h"

LineEdit::LineEdit(const QString& label, QWidget *parent) : TemplateWidget(parent) {
    setLabel(label);
    edit = new QLineEdit;
    addWidget(edit);
    
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