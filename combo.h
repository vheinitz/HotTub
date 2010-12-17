
#ifndef COMBO_H
#define COMBO_H

#include "templatewidget.h"

class Combo : public Editor {
    
public:
    Combo(QWidget *);
    
    void enterEditMode();
    void exitEditMode();
    void setActiveCursor(Qt::CursorShape);
    bool allowResizeWidth();
    bool allowResizeHeight();
    void setMargins(int,int,int,int);
    QSize sizeHint();
    int getLeftAlignmentHint();
private:
    QComboBox *edit;
    QVBoxLayout* layout;
    
};




#endif