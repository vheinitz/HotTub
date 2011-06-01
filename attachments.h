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

#ifndef __ATTACHMENTS_H
#define __ATTACHMENTS_H

#include <QtGui>
#include "qcouch/document.h"
#include "qcouch/qcouch.h"

enum AttachmentType {
    ATTACHMENT,
    FOLDER
};

typedef struct {
    QString name;
    AttachmentType type;
    QUrl url;
    QString database;
    QString id;
    QString revision;
    QString downloadFileName;
} AttachedObject;

class Attachments : public QWidget {
    Q_OBJECT
    
public:
    Attachments(QCouch& couch, QWidget *parent);
    QSize sizeHint();
    void paintEvent(QPaintEvent *event);
    void acceptUrl(QUrl url);
	void acceptFileByName(QString filename);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void loadDocument(Document &);
signals:
    void fileAttached(QUrl url);
    
private slots:
    void addFolder();
    void openObject();
    void deleteObject();
	void addButtonClicked();
    
private:
    QCouch& couch;
    bool hasSelectedObject();
    
    QList<AttachedObject> files;
  
    static const int iconWidth = 44;
    static const int iconHeight = 57;
    
    int columnWidth;
    int columnHeight;
    int rowHeight;
    int columns;
    int leftmargin;
    int selectedColumn;
    
    QMenu *contextMenu;
    QPoint dragStartPosition;
    
    QAction *openAction;
    QAction *deleteAction;
    QAction *newFolderAction;

};



#endif
