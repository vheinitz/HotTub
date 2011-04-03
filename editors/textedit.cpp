

#include "textedit.h"

TextEdit::TextEdit(QWidget *parent) : Editor(parent) {
    edit = new QTextEdit;
    layout = new QVBoxLayout;
    layout->addSpacing(4);
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
    return edit->sizeHint();
}


int TextEdit::getLeftAlignmentHint(){
    return edit->x();
}

void TextEdit::loadDocument(Document doc){
    QVariantMap map = doc.getMap();
    QVariant val = map[getField()];
    originalValue = val.toString();
    edit->setText(val.toString());
    
    setBackgroundColor(QColor(255,255,255));
    connect(edit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    
}

bool TextEdit::hasChanges(){
    return originalValue != edit->toPlainText();
}

void TextEdit::saveChanges(Document& doc){
    doc.setValue(getField(), QVariant(edit->toPlainText()));
}

void TextEdit::reset(){
    originalValue = "";
    edit->clear();
}

QString TextEdit::type(){   
    return "textedit";
}


void TextEdit::setBackgroundColor(QColor color){
    QPalette pal = edit->palette();
    pal.setColor(QPalette::Base, color);
    edit->setPalette(pal);
    
}

void TextEdit::textChanged(){
    setBackgroundColor(QColor(255,229,229));
}
