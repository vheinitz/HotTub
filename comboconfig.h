

#ifndef __COMBOCONFIG_H
#define __COMBOCONFIG_H


#include <QtGui>



class ComboConfigurationDialog : public QDialog {
    Q_OBJECT
public:
    ComboConfigurationDialog();
    QStringList stringList();
    
private slots:
    void addItem();
    void okButtonClicked();
    void cancelButtonClicked();
    
private:
    QStringListModel* model;
    QStringList list;
    QLineEdit* edit;
    
};


#endif