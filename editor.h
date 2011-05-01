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
    virtual void setDisabled(bool enable) = 0;
    void setField(QString);
    QString getField();
    
    virtual bool hasChanges() = 0;
    virtual void loadDocument(Document doc) = 0;
    virtual void saveChanges(Document& doc) = 0;
    virtual void reset() = 0;
    virtual void addConfigurationAction(QToolBar *toolbar);
    virtual void removeConfigurationAction(QToolBar *toolbar);
    virtual void saveConfiguration(QVariantMap &);
    virtual void loadConfiguration(QVariant&);

};





#endif
