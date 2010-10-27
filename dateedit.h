
#ifndef DATEEDIT_H
#define DATEEDIT_H

#include "templatewidget.h"

class DateEdit : public TemplateWidget {
    
public:
    DateEdit(const QString& label, QWidget *parent);
    
    void enterEditMode();
    void exitEditMode();
    void setActiveCursor(Qt::CursorShape);
    int getLeftAlignmentHint();
    int getTopAlignmentHint();
    bool allowResizeWidth();
    bool allowResizeHeight();

private:
    QDateTimeEdit *edit;
    
};




#endif