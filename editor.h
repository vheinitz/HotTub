


#ifndef __EDITOR_H
#define __EDITOR_H

#include <QtGui>

class Editor : public QWidget {
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
};












#endif
