

#ifndef HOTSPOT__H
#define HOTSPOT__H

#include <QWidget>
#include <QRect>
#include "action.h"

class Hotspot {
public:
    Hotspot(QWidget*, QRect, Qt::CursorShape, Action);
    
    QRect rect();
    QWidget* targetWidget();
    Qt::CursorShape getCursor();
    Action getAction();
private:
    QWidget* widget;
    QRect rectangle;
    Qt::CursorShape cursor;
    Action action;
};




#endif