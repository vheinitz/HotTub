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


#include "listedit.h"

ListEdit::ListEdit(QWidget* parent) : Editor(parent){
    view = new QListView;
    edit = new QLineEdit;
    model = new QStringListModel();
    model->setStringList(list);    
    view->setModel(model);
    
    layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(view);
    layout->addWidget(edit);

    setLayout(layout);

    changed = false;
    connect(edit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
}

void ListEdit::returnPressed(){
    if ( edit->text().length() > 0 ){
        list << edit->text();
        model->setStringList(list);
        edit->clear();
        changed = true;
    }
}

void ListEdit::enterEditMode(){
}

void ListEdit::exitEditMode(){
}

void ListEdit::setActiveCursor(Qt::CursorShape){
}

bool ListEdit::allowResizeWidth(){
    return true;
}

bool ListEdit::allowResizeHeight(){
    return true;
}

void ListEdit::setMargins(int left,int top,int right,int bottom){
    layout->setContentsMargins(left,top,right,bottom);
}

QSize ListEdit::sizeHint(){
    return layout->sizeHint();
}

int ListEdit::getLeftAlignmentHint(){
    return view->x();
}

QString ListEdit::type(){
    return "listedit";
}

void ListEdit::loadDocument(Document doc){
    QVariantMap map = doc.getMap();
    QVariant val = map[getField()];
    qDebug() << val;
    list.clear();
    list << val.toStringList();
    model->setStringList(list);
}

void ListEdit::saveChanges(Document& doc){
    doc.setValue(getField(), QVariant(list));
}

bool ListEdit::hasChanges(){
    return changed;
}

void ListEdit::reset(){
    list.clear();
    model->setStringList(list);
}

