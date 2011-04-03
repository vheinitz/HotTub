
#ifndef LINEEDIT_H
#define LINEEDIT_H

#include "templatewidget.h"
#include "qcouch/document.h"

class LineEdit : public Editor {
    Q_OBJECT
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
    int getLeftAlignmentHint();
    QString type();
    void loadDocument(Document doc);
    void saveChanges(Document& doc);
    bool hasChanges();
    void reset();
private slots:
    void textChanged(const QString &);
private:
    void setBackgroundColor(QColor);
    
    QLineEdit *edit;
    QLayout *layout;
    
    QString originalValue;
};




#endif