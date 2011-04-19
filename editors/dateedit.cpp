
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
    return edit->sizeHint();
}   

int DateEdit::getLeftAlignmentHint(){
    return edit->x();
}

void DateEdit::loadDocument(Document doc){
    QVariantMap map = doc.getMap();
    QVariant val = map[getField()];

    bool ok;
    uint secs = val.toUInt(&ok);
    if ( ok ){
        edit->setDateTime(QDateTime::fromTime_t(secs));
    }
    originalValue = edit->dateTime().toTime_t();
   
}

void DateEdit::saveChanges(Document& doc){
    doc.setValue(getField(), QVariant(edit->dateTime().toTime_t()));
}


bool DateEdit::hasChanges(){
    return originalValue != edit->dateTime().toTime_t();
}

void DateEdit::reset(){

}

QString DateEdit::type(){  
    return "dateedit";

}