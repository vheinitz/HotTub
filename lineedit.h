
#ifndef LINEEDIT_H
#define LINEEDIT_H

#include "templatewidget.h"

class LineEdit : public Editor {
    
public:
    LineEdit(QWidget *parent);
    
    void enterEditMode();
    void exitEditMode();
    void setActiveCursor(Qt::CursorShape);
    bool allowResizeWidth();
    bool allowResizeHeight();
    void setMargins(int,int,int,int);
    void paintEvent(QPaintEvent *event);
    QSize sizeHint();
private:
    QLineEdit *edit;
    QLayout *layout;
};




#endif