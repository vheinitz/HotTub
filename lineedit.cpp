

#include "lineedit.h"

LineEdit::LineEdit(const QString& label, QWidget *parent) : TemplateWidget(parent) {
    setLabel(label);
    edit = new QLineEdit;
    setMinimumSize(285, 65);
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

int LineEdit::getLeftAlignmentHint(){
    return edit->x();
}

int LineEdit::getTopAlignmentHint(){
    return edit->y();
}

bool LineEdit::allowResizeWidth(){
    return true;
}

bool LineEdit::allowResizeHeight(){
    return false;
}