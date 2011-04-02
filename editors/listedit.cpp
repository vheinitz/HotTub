


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

