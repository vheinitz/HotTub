
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
