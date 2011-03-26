

#ifndef __LISTCONFIG_H
#define __LISTCONFIG_H


#include <QtGui>



class ListConfigurationDialog : public QDialog {
    Q_OBJECT
public:
    ListConfigurationDialog();
    QStringList stringList();
    void addAll(QStringList&);
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