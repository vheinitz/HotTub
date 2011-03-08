
#ifndef __MAIN_H
#define __MAIN_H

#include <QApplication>
#include <QtGui>
#include "qcouch/document.h"
#include "qcouch/qcouch.h"

#include "list.h"
#include "model.h"
#include "view.h"

class MainWindow : public QWidget {
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    
public slots:
    void databaseSelected(const QString &);    
    void loadSelectedView(const QString &);
    void listSelectionChanged(int);
    void documentUpdated(Document &);
    void documentDeleted(Document &);
    void documentAdded(Document &);
    
private:
    void doDatabaseSelection();
    void getViews(QString&);
    
    QDialog *dlg;
    QComboBox *viewsCombo;
    QString selectedDatabase;
    List *list;
    Model *model;
    View *view;
    
    QCouch couch;
};

#endif
