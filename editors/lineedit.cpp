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

#include "lineedit.h"
#include "qcouch/document.h"
#include <QDebug>
using namespace std;

LineEdit::LineEdit(QWidget *parent) : Editor(parent) {
    edit = new QLineEdit; 
    layout = new QHBoxLayout;
    layout->addWidget(edit);
    setLayout(layout);
    
}

void LineEdit::setMargins(int top, int right, int bottom, int left){
    layout->setContentsMargins(top,right,bottom,left);
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

bool LineEdit::allowResizeWidth(){
    return true;
}

bool LineEdit::allowResizeHeight(){
    return false;
}

void LineEdit::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
}

QSize LineEdit::sizeHint() {
    return edit->sizeHint();
}

int LineEdit::getLeftAlignmentHint(){
    return edit->x();
}


bool LineEdit::hasChanges(){
    return originalValue != edit->text();
}

void LineEdit::loadDocument(Document doc){
    if ( getField() == "_id" ) {
        edit->setText(doc.getId());
    } else {
        QVariantMap map = doc.getMap();
        QVariant val = map[getField()];
        edit->setText(val.toString());
    }
    originalValue = edit->text();
    setBackgroundColor(QColor(255,255,255));
    connect(edit, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));

}

void LineEdit::saveChanges(Document& doc){
    doc.setValue(getField(), QVariant(edit->text()));
}

void LineEdit::reset(){
    edit->setText("");
}

QString LineEdit::type(){
    return "lineedit";
}

void LineEdit::setBackgroundColor(QColor color){
    QPalette pal = edit->palette();
    pal.setColor(edit->backgroundRole(), color);
    edit->setPalette(pal);
    
}

void LineEdit::textChanged(const QString &text){
    Q_UNUSED(text);
    setBackgroundColor(QColor(255,229,229));
}



void LineEdit::setDisabled(bool disabled){
    edit->setDisabled(disabled);
}


