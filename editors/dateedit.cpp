/*
 This file is part of Hot Tub, a project to help interactions with Apache CouchDB
 Copyright 2011 Matt Jennings <jennmat@gmail.com>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3.0 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
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