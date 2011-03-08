
#ifndef DATEEDIT_H
#define DATEEDIT_H

#include "editor.h"
#include "qcouch/document.h"

class DateEdit : public Editor {
    
public:
    DateEdit(QWidget *parent);
    
    void enterEditMode();
    void exitEditMode();
    void setActiveCursor(Qt::CursorShape);
    bool allowResizeWidth();
    bool allowResizeHeight();
    void setMargins(int,int,int,int);
    QSize sizeHint();
    int getLeftAlignmentHint();
    
    void setField(const QString &);
    void loadDocument(Document doc);
    void saveChanges(Document& doc);
    
    bool hasChanges();
private:
    QDateTimeEdit *edit;
    QLayout *layout;
    
};




#endif