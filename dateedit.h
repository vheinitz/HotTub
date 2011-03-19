
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
    QString type();
    bool hasChanges();
    void reset();
private:
    QDateTimeEdit *edit;
    QLayout *layout;
    
};




#endif