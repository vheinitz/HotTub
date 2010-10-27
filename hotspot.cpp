
#include <QWidget>
#include <QRect>

#include "hotspot.h"
#include "action.h"

Hotspot::Hotspot(QWidget* w, QRect rect, Qt::CursorShape c, Action a) {
    widget = w;
    rectangle = rect;
    cursor = c;
    action = a;
}

QRect Hotspot::rect(){
    return rectangle;
}

QWidget* Hotspot::targetWidget(){
    return widget;
}

Qt::CursorShape Hotspot::getCursor(){
    return cursor;
}

Action Hotspot::getAction(){
    return action;
}

