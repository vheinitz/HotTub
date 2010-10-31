

#include "textedit.h"

TextEdit::TextEdit(const QString& label, QWidget *parent) : TemplateWidget(parent) {
    setLabel(label);
    edit = new QTextEdit;
    setMinimumSize(300,80);
    addWidget(edit);
}

void TextEdit::enterEditMode(){
    edit->setDisabled(true);
}

void TextEdit::exitEditMode(){
    edit->setDisabled(false);
    edit->viewport()->setCursor(Qt::IBeamCursor);
}

void TextEdit::setActiveCursor(Qt::CursorShape shape){
    edit->viewport()->setCursor(shape);    
}

int TextEdit::getLeftAlignmentHint(){
    return edit->x();
}

int TextEdit::getTopAlignmentHint(){
    return edit->y();
}

bool TextEdit::allowResizeWidth(){
    return true;
}

bool TextEdit::allowResizeHeight(){
    return true;
}


