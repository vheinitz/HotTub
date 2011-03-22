
#include "combo.h"
#include "qcouch/document.h"
#include "comboconfig.h"


#include <QtGui>

Combo::Combo(QWidget *parent) : Editor(parent) {
    edit = new QComboBox;
    layout = new QHBoxLayout;
    layout->addWidget(edit);
    setLayout(layout);
    
    dlg = new ComboConfigurationDialog();
    connect(dlg, SIGNAL(accepted()), this, SLOT(comboConfigAccepted()));
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
    return true;
}

QSize Combo::sizeHint(){
    return edit->sizeHint();
}

int Combo::getLeftAlignmentHint(){
    return edit->x();
}

void Combo::configurationAction(QToolBar *toolbar){
    QAction* action = new QAction(toolbar);
    action->setIcon(QIcon("icons/items.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(configureItems()));
    toolbar->addAction(action);
}

void Combo::configureItems(){
    dlg->show();
}

void Combo::comboConfigAccepted(){
    edit->insertItems(0, dlg->stringList());
}

void Combo::loadDocument(Document doc){
    
    QVariantMap map = doc.getMap();
    QVariant val = map[getField()];
    int index = edit->findText(val.toString());
    if ( index >= 0 ) {
        edit->setCurrentIndex(index);
    }
    
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
}

bool Combo::hasChanges(){
    return false;
}

void Combo::reset() {
    
}

QString Combo::type(){
    return "combo";
}




            
            



