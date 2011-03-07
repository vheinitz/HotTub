

#include "lineedit.h"
#include "qcouch/document.h"

using namespace std;

LineEdit::LineEdit(QWidget *parent) : Editor(parent) {
    edit = new QLineEdit; 
    layout = new QHBoxLayout;
    layout->addWidget(edit);
    setLayout(layout);
}

void LineEdit::setMargins(int top, int right, int bottom, int left){
    layout->setContentsMargins(top,right,bottom,left);
}

void LineEdit::enterEditMode(){
    edit->setDisabled(true);
}

void LineEdit::exitEditMode(){
    edit->setDisabled(false);
    edit->setCursor(Qt::IBeamCursor);
}

void LineEdit::setActiveCursor(Qt::CursorShape shape){
    edit->setCursor(shape);
}

bool LineEdit::allowResizeWidth(){
    return true;
}

bool LineEdit::allowResizeHeight(){
    return false;
}

void LineEdit::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
}

QSize LineEdit::sizeHint() {
    return edit->sizeHint();
}

int LineEdit::getLeftAlignmentHint(){
    return edit->x();
}



void LineEdit::loadDocument(Document doc){
    QVariantMap map = doc.getMap();
    QVariant val = map[getField()];
    edit->setText(val.toString());
    
    /*Variant v = doc.getData();
    Object obj = boost::any_cast<Object>(*v);
    
    string f = getField().toStdString();
    
    if ( obj.find(f) == obj.end() ) {
        return;
    }
    boost::any val = *obj[f];
    const type_info &t = val.type();
    if(t == typeid(string)) {
        string s = boost::any_cast<string>(val);
        edit->setText(QString(s.c_str()));
    } else if(t == typeid(int))
        edit->setText(QString(boost::any_cast<int>(val)));
        */
}
