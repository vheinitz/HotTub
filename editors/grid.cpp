
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
#include "grid.h"
#include "gridmodel.h"
#include "listconfig.h"
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QHeaderView>
#include <QDebug>

Grid::Grid(QWidget* parent) : Editor(parent){
    view = new QTableView;
    
        
    layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(view);
    
    setLayout(layout);
    
    dlg = new ListConfigurationDialog();
    connect(dlg, SIGNAL(accepted()), this, SLOT(columnsConfigAccepted()));

    action = NULL;
    model = NULL;
}

void Grid::setColumnHeaders(QStringList headers ){
    dlg->addAll(headers);
    if ( model == NULL ) {
        model = new GridModel(QVariant(), headers);
		connect(model, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
        view->setModel(model);
    } else {
        model->setHeaders(headers);
    }
    
}

void Grid::columnsConfigAccepted(){
    headers.clear();
    headers << dlg->stringList();
    if ( model == NULL ) {
        model = new GridModel(QVariant(), headers);
		connect(model, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
        view->setModel(model);
    } else {
        model->setHeaders(headers);
    }
}

void Grid::dataChanged(){
    setBackgroundColor(QColor(255,229,229));
}

void Grid::addConfigurationAction(QToolBar *toolbar){
    action = new QAction(toolbar);
    action->setIcon(QIcon("icons/items.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(configureItems()));
    toolbar->addAction(action);
}

void Grid::removeConfigurationAction(QToolBar *toolbar){
    if ( action != NULL ){
        toolbar->removeAction(action);
    }
}

void Grid::configureItems(){
    dlg->show();
}


void Grid::enterEditMode(){
    view->setDisabled(true);
}

void Grid::exitEditMode(){
    view->setDisabled(false);
}

void Grid::setActiveCursor(Qt::CursorShape shape){
	view->setCursor(shape);
}


bool Grid::allowResizeWidth(){
    return true;
}

bool Grid::allowResizeHeight(){
    return true;
}

void Grid::setMargins(int left,int top,int right,int bottom){
    layout->setContentsMargins(left,top,right,bottom);
}

QSize Grid::sizeHint(){
    return layout->sizeHint();
}

int Grid::getLeftAlignmentHint(){
    return view->x();
}

QString Grid::type(){
    return "grid";
}

void Grid::loadDocument(Document doc){
    QVariantMap map = doc.getMap();
    QVariant val = map[getField()];
    
    if ( headers.size() == 0 ) {
        /* No template configured for this grid, infer from data */
        QList<QVariant> list = val.toList();
        if ( list.size() > 0 ) {
            QVariant var = list[0];
            QVariantMap map = var.toMap();
            headers << map.keys();
            dlg->addAll(headers);
        }    
    }
    
    model = new GridModel(val, headers);
	connect(model, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    view->setModel(model);
    view->resizeColumnsToContents();
	setBackgroundColor(QColor(255,255,255));
}

void Grid::saveChanges(Document& doc){
    doc.setValue(getField(), model->getVariant());
}

bool Grid::hasChanges(){
    return model != NULL && model->hasChanges();
}

void Grid::reset(){
}


void Grid::saveConfiguration(QVariantMap& map){
    map["values"] = QVariant(headers);
}

void Grid::loadConfiguration(QVariant& var){
    QVariantMap map = var.toMap();
    QVariant values = map["values"];
    headers.clear();
    headers << values.toStringList();
    setColumnHeaders(headers);
}

void Grid::setBackgroundColor(QColor color){
    QPalette pal = view->palette();
    pal.setColor(QPalette::Base, color);
    view->setPalette(pal);
}


