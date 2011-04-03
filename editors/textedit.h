
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "templatewidget.h"
#include "qcouch/document.h"

class TextEdit : public Editor {
    Q_OBJECT
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
    QString type();
    void loadDocument(Document doc);
    void saveChanges(Document& doc);
    bool hasChanges();
    void reset();
private slots:
    void textChanged();
private:
    void setBackgroundColor(QColor);
    
    QTextEdit *edit;
    QVBoxLayout* layout;
    
    QString originalValue;
};




#endif