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