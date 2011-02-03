
#include <QApplication>
#include "view.h"

#include <QtGui>
#include "list.h"
#include "view.h"

#include "couchdb/CouchDB.hpp"

using namespace CouchDB;
using namespace std;

class MainWindow : public QWidget {
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);

public slots:
    void databaseSelected(const QString &text);    
    
private:
    void doDatabaseSelection(Connection &conn);
};

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setMinimumSize(1200, 600);
    
    View *view = new View(this);
    List *list = new List(this);
    
    view->setMinimumWidth(600);
    list->setMinimumWidth(600);
    
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(5,5,5,5);
    layout->addWidget(list, 0, 0);
    layout->addWidget(view, 0, 1);
    setLayout(layout);
    
    Connection conn;
    doDatabaseSelection(conn);
    
}

void MainWindow::databaseSelected(const QString &text){
    QMessageBox box;
    box.setText(text);
    box.show();
}

void MainWindow::doDatabaseSelection(Connection &conn){
    vector<string> dbs = conn.listDatabases();
    
    QDialog dlg;
    QFormLayout layout;
    
    QComboBox *combo = new QComboBox(&dlg);
    for(unsigned int i=0; i<dbs.size(); i++){
        combo->insertItem(i, QString(dbs[i].c_str()));
    }
    
    connect(combo, SIGNAL(currentIndexChanged(const QString &text)), this, SLOT(databaseSelected(const QString &text)));
    
    layout.addRow(new QLabel("Select a database"), combo);
    dlg.setLayout(&layout);
    dlg.setFixedSize(350, 100);            
    dlg.exec();
                             
}




int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    
    
    
    return app.exec();
}
