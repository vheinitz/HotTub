
#include "editor.h"
#include <QtGui>

Editor::Editor(QWidget* parent) : QWidget(parent) {

}


void Editor::addConfigurationAction(QToolBar *toolbar){
    Q_UNUSED(toolbar);
}

void Editor::removeConfigurationAction(QToolBar *toolbar){
    Q_UNUSED(toolbar);
}


void Editor::setField(QString _field){
    field = _field;
}


QString Editor::getField(){
    return field;
}

void Editor::saveConfiguration(QVariantMap& map){
    Q_UNUSED(map);

}

void Editor::loadConfiguration(QVariant& var){
    Q_UNUSED(var);
}
