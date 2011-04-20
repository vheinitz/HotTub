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
#ifndef __MAIN_H
#define __MAIN_H

#include <QApplication>
#include <QtGui>
#include "qcouch/document.h"
#include "qcouch/qcouch.h"

#include "list.h"
#include "model.h"
#include "view.h"
#include "viewconfig.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    MainWindow();
    
public slots:
    void databaseSelected(const QString &);    
    void loadSelectedView(const QString &);
    void listSelectionChanged(int);
    void documentUpdated(Document &);
    void documentDeleted(Document &);
    void documentAdded(Document &);
    void startKeyChanged(const QString &);    
    void endKeyChanged(const QString &);
    void updateView();
    void descendingCheckStateChanged(int);
    void configureViewColumns();
    void columnConfigAccepted();
    void nextPage();
    void previousPage();
private:
    void doDatabaseSelection();
    void getViews(QString&);
    void loadViewConfiguration();
    void closeEvent(QCloseEvent *event);
    void removeDirectory(QString);
    
    QDialog *dlg;
    QComboBox *viewsCombo;
    QLineEdit *startKeyEdit;
    QLineEdit *endKeyEdit;
    QCheckBox *descending;
    
    QPushButton *nextPageButton;
    QPushButton *previousPageButton;

    QString selectedDatabase;
    List *list;
    Model *model;
    View *view;

    ViewConfig *viewConfig;

    QString design;
    QString currentView;
    
    QCouch couch;
    QTimer keyTimer;
};

#endif
