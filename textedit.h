
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "templatewidget.h"

class TextEdit : public TemplateWidget {
 
public:
    TextEdit(const QString& label, QWidget *parent);
    
    void enterEditMode();
    void exitEditMode();
    void setActiveCursor(Qt::CursorShape);
private:
    QTextEdit *edit;
    
};




#endif