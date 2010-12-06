

#include "textedit.h"

TextEdit::TextEdit(QWidget *parent) : Editor(parent) {
    edit = new QTextEdit;
    layout = new QHBoxLayout;
    layout->addWidget(edit);
    setLayout(layout);
}

void TextEdit::setMargins(int top, int right, int bottom, int left){
    layout->setContentsMargins(top,right,bottom,left);
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

bool TextEdit::allowResizeWidth(){
    return true;
}

bool TextEdit::allowResizeHeight(){
    return true;
}

QSize TextEdit::sizeHint(){
    return QSize(400, 140);
}

