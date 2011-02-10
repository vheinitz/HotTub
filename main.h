
#ifndef __MAIN_H
#define __MAIN_H

#include <QApplication>
#include <QtGui>
#include "couchdb/CouchDB.hpp"

using namespace CouchDB;

class MainWindow : public QWidget {
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    
public slots:
    void databaseSelected(const QString &);    
    
private:
    void doDatabaseSelection(Connection &conn);
    void getViews(Database &);
    
    QDialog *dlg;
    QComboBox *viewsCombo;
    QString selectedDatabase;
};

#endif
