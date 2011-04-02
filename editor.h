
#ifndef __EDITOR_H
#define __EDITOR_H

#include <QtGui>
#include "qcouch/document.h"


class Editor : public QWidget {
private:
    QString field;

public:
    Editor(QWidget *parent);
    virtual bool allowResizeWidth() = 0;
    virtual bool allowResizeHeight() = 0;
    virtual void setActiveCursor(Qt::CursorShape) = 0;
    virtual void enterEditMode() = 0;
    virtual void exitEditMode() = 0;
    virtual void setMargins(int,int,int,int) = 0;
    virtual QSize sizeHint() = 0;
    virtual int getLeftAlignmentHint() = 0;
    virtual QString type() = 0;
    void setField(QString);
    QString getField();
    
    virtual bool hasChanges() = 0;
    virtual void loadDocument(Document doc) = 0;
    virtual void saveChanges(Document& doc) = 0;
    virtual void reset() = 0;
    virtual void configurationAction(QToolBar *toolbar);
    virtual void saveConfiguration(QVariantMap &);
    virtual void loadConfiguration(QVariant&);

};





#endif
