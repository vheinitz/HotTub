

#include "grid.h"
#include "gridmodel.h"
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
    
}

void Grid::enterEditMode(){
}

void Grid::exitEditMode(){
}

void Grid::setActiveCursor(Qt::CursorShape){
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
    
    model = new GridModel(val);
    view->setModel(model);
}

void Grid::saveChanges(Document& doc){
    doc.setValue(getField(), model->getVariant());
}

bool Grid::hasChanges(){
    return false;
}

void Grid::reset(){
}

