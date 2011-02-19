
#ifndef __MAIN_H
#define __MAIN_H

#include <QApplication>
#include <QtGui>
#include "couchdb/CouchDB.hpp"

#include "list.h"
#include "model.h"
#include "view.h"

using namespace CouchDB;

class MainWindow : public QWidget {
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    
public slots:
    void databaseSelected(const QString &);    
    void loadSelectedView(const QString &);
    void listSelectionChanged(int);
private:
    void doDatabaseSelection(Connection &conn);
    void getViews(Database &);
    
    QDialog *dlg;
    QComboBox *viewsCombo;
    QString selectedDatabase;
    List *list;
    Model *model;
    View *view;
    Connection conn;
};

#endif
