

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
    
}

void Grid::setColumnHeaders(QStringList headers ){
    dlg->addAll(headers);
    if ( model == NULL ) {
        model = new GridModel(QVariant(), headers);
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
        view->setModel(model);
    } else {
        model->setHeaders(headers);
    }
}


void Grid::configurationAction(QToolBar *toolbar){
    QAction* action = new QAction(toolbar);
    action->setIcon(QIcon("icons/items.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(configureItems()));
    toolbar->addAction(action);
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
    view->setModel(model);
    view->resizeColumnsToContents();
}

void Grid::saveChanges(Document& doc){
    doc.setValue(getField(), model->getVariant());
}

bool Grid::hasChanges(){
    return model->hasChanges();
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

