
#include "editor.h"
#include <QtGui>

Editor::Editor(QWidget* parent) : QWidget(parent) {

}


QAction* Editor::configurationAction(QToolBar *toolbar){
    Q_UNUSED(toolbar);
    return NULL;
}