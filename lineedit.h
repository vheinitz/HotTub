
#ifndef LINEEDIT_H
#define LINEEDIT_H

#include "templatewidget.h"

class LineEdit : public TemplateWidget {
    
public:
    LineEdit(const QString& label, QWidget *parent);
    
    void enterEditMode();
    void exitEditMode();
    void setActiveCursor(Qt::CursorShape);
private:
    QLineEdit *edit;
    
};




#endif