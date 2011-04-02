
#ifndef __COMBO_H
#define __COMBO_H

#include "templatewidget.h"
#include "qcouch/document.h"
#include "listconfig.h"
#include <QtGui>




class Combo : public Editor {
    Q_OBJECT
public:
    Combo(QWidget *);
    
    void enterEditMode();
    void exitEditMode();
    void setActiveCursor(Qt::CursorShape);
    bool allowResizeWidth();
    bool allowResizeHeight();
    void setMargins(int,int,int,int);
    QSize sizeHint();
    int getLeftAlignmentHint();
    void configurationAction(QToolBar *toolbar);
    QString type();
    void loadDocument(Document doc);
    void saveChanges(Document& doc);
    void reset();
    bool hasChanges();
    void saveConfiguration(QVariantMap& map);
    void loadConfiguration(QVariant&);
    void paintEvent(QPaintEvent* event);
    bool isDirty();
private slots:
    void configureItems();
    void comboConfigAccepted();
private:
    QComboBox *edit;
    QHBoxLayout* layout;
    
    ListConfigurationDialog* dlg;
    int originalIndex;
};




#endif

