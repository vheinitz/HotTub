

#include "lineedit.h"
#include "qcouch/document.h"
#include <QDebug>
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


bool LineEdit::hasChanges(){
    return originalValue != edit->text();
}

void LineEdit::loadDocument(Document doc){
    if ( getField() == "_id" ) {
        edit->setText(doc.getId());
    } else {
        QVariantMap map = doc.getMap();
        QVariant val = map[getField()];
        edit->setText(val.toString());
    }
    originalValue = edit->text();
}

void LineEdit::saveChanges(Document& doc){
    doc.setValue(getField(), QVariant(edit->text()));
}

void LineEdit::reset(){
    edit->setText("");
}

QString LineEdit::type(){
    return "lineedit";
}
