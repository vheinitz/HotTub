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


#ifndef __GRID_H
#define __GRID_H

#include "editor.h"
#include "gridmodel.h"
#include "qcouch/document.h"
#include "listconfig.h"



class Grid : public Editor {
    Q_OBJECT
public:
    Grid(QWidget *);
    
    
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
    void addConfigurationAction(QToolBar *toolbar);
    void removeConfigurationAction(QToolBar *toolbar);
    
    void saveConfiguration(QVariantMap& map);
    void loadConfiguration(QVariant&);
    void setColumnHeaders(QStringList);
    void setDisabled(bool);

private slots:
    void columnsConfigAccepted();
    void configureItems();
	void dataChanged();
private:
	void setBackgroundColor(QColor color);
    QTableView *view;
    QHBoxLayout *layout;
    GridModel *model;
    QStringList headers;
    ListConfigurationDialog *dlg;
	
	QAction *action;
};


#endif
