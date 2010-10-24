

#include "textedit.h"

TextEdit::TextEdit(const QString& label, QWidget *parent) : TemplateWidget(parent) {
    setLabel(label);
    edit = new QTextEdit;
    edit->setFixedSize(245,65);
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