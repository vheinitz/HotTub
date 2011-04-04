


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
    void configurationAction(QToolBar *toolbar);
    
    void saveConfiguration(QVariantMap& map);
    void loadConfiguration(QVariant&);
    void setColumnHeaders(QStringList);
private slots:
    void columnsConfigAccepted();
    void configureItems();

private:
    QTableView *view;
    QHBoxLayout *layout;
    GridModel *model;
    QStringList headers;
    ListConfigurationDialog *dlg;
};



#endif