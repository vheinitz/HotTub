/*
 This file is part of Hot Tub, a project to help interactions with Apache CouchDB
 Copyright 2011 Matt Jennings <jennmat@gmail.com>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3.0 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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