
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "templatewidget.h"

class TextEdit : public Editor {
 
public:
    TextEdit(QWidget *);
    
    void enterEditMode();
    void exitEditMode();
    void setActiveCursor(Qt::CursorShape);
    bool allowResizeWidth();
    bool allowResizeHeight();
    void setMargins(int,int,int,int);
    QSize sizeHint();
    int getLeftAlignmentHint();
private:
    QTextEdit *edit;
    QVBoxLayout* layout;
    
};




#endif