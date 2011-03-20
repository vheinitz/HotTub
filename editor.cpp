
#include "editor.h"
#include <QtGui>

Editor::Editor(QWidget* parent) : QWidget(parent) {

}


void Editor::configurationAction(QToolBar *toolbar){
    Q_UNUSED(toolbar);
}


void Editor::setField(QString _field){
    field = _field;
}


QString Editor::getField(){
    return field;
}

void Editor::saveConfiguration(QVariantMap& map){

}

void Editor::loadConfiguration(QVariant& var){

}