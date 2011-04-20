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
#include "combo.h"
#include "qcouch/document.h"
#include "listconfig.h"

#include <QtGui>

Combo::Combo(QWidget *parent) : Editor(parent) {
    edit = new QComboBox;
    layout = new QHBoxLayout;
    layout->addWidget(edit);
	layout->addStretch();
    setLayout(layout);
    
    dlg = new ListConfigurationDialog();
    connect(dlg, SIGNAL(accepted()), this, SLOT(comboConfigAccepted()));

    action = NULL;
}

void Combo::setMargins(int top, int right, int bottom, int left){
    layout->setContentsMargins(top,right,bottom,left);
}

void Combo::paintEvent(QPaintEvent* event){
    QWidget::paintEvent(event);
    QPainter painter(this);
}

void Combo::enterEditMode(){
    edit->setDisabled(true);
}

void Combo::exitEditMode(){
    edit->setDisabled(false);
    edit->setCursor(Qt::IBeamCursor);
}

void Combo::setActiveCursor(Qt::CursorShape shape){
    edit->setCursor(shape);    
}

bool Combo::allowResizeWidth(){
    return true;
}

bool Combo::allowResizeHeight(){
    return false;
}

QSize Combo::sizeHint(){
    return edit->sizeHint();
}

int Combo::getLeftAlignmentHint(){
    return edit->x();
}

void Combo::addConfigurationAction(QToolBar *toolbar){
    action = new QAction(toolbar);
    action->setIcon(QIcon("icons/items.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(configureItems()));
    toolbar->addAction(action);
}

void Combo::removeConfigurationAction(QToolBar *toolbar){
	if ( action != NULL ){
        toolbar->removeAction(action);
    }    
}

void Combo::configureItems(){
    dlg->show();
}

void Combo::comboConfigAccepted(){
    edit->clear();
    edit->insertItems(0, dlg->stringList());
}

void Combo::loadDocument(Document doc){
    
    QVariantMap map = doc.getMap();
    QVariant val = map[getField()];
    int index = edit->findText(val.toString());
    if ( index >= 0 ) {
        edit->setCurrentIndex(index);
    } else {
        edit->setCurrentIndex(-1);
    }
    originalIndex = edit->currentIndex();
    
    
}

void Combo::saveChanges(Document& doc){
    if ( edit->currentIndex() >= 0 ) {
        doc.setValue(getField(), edit->currentText());
    }
}

void Combo::saveConfiguration(QVariantMap& map){
    QStringList items;
    for (int i=0; i<edit->count(); i++) {
        qDebug() << edit->itemText(i);
        items.append(edit->itemText(i));
    }
    map["values"] = QVariant(items);
}

void Combo::loadConfiguration(QVariant& var){
    
    QVariantMap map = var.toMap();
    QVariant values = map["values"];
    
    QStringList items = values.toStringList();
    edit->insertItems(0, items);
    dlg->addAll(items);
}

bool Combo::hasChanges(){
    return originalIndex != edit->currentIndex();
}

void Combo::reset() {
    
}

QString Combo::type(){
    return "combo";
}




            
            



