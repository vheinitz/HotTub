
#ifndef __COMBO_H
#define __COMBO_H

#include "templatewidget.h"
#include "qcouch/document.h"
#include <QtGui>

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
    QAction* configurationAction(QToolBar *toolbar);

    void loadDocument(Document doc);
    
private:
    QComboBox *edit;
    QVBoxLayout* layout;
    
};




#endif

