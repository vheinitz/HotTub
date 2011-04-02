


#ifndef __LISTEDIT_H
#define __LISTEDIT_H

#include "editor.h"
#include "qcouch/document.h"



class ListEdit : public Editor {
    Q_OBJECT
public:
    ListEdit(QWidget *);
    

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
    void returnPressed();
private:
    QListView *view;
    QVBoxLayout *layout;
    QLineEdit *edit;
    QStringList list;
    QStringListModel *model;
    
    bool changed;
};



#endif