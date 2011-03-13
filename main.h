
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
    void startKeyChanged(const QString &);    
    void endKeyChanged(const QString &);
    void updateView();
private:
    void doDatabaseSelection();
    void getViews(QString&);
    
    QDialog *dlg;
    QComboBox *viewsCombo;
    QLineEdit *startKeyEdit;
    QLineEdit *endKeyEdit;

    QString selectedDatabase;
    List *list;
    Model *model;
    View *view;

    QString design;
    QString currentView;
    
    QCouch couch;
    QTimer keyTimer;
};

#endif
