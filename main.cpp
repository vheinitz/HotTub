
#include <QApplication>
#include "view.h"

#include <QtGui>
#include "list.h"
#include "view.h"
#include "main.h"
#include "model.h"
#include "app.h"

#include "couchdb/CouchDB.hpp"

using namespace CouchDB;
using namespace std;


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setMinimumSize(1200, 600);
    
    View *view = new View(this);
    list = new List(this);
    
    view->setMinimumWidth(600);
    list->setMinimumWidth(600);
    
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(5,5,5,5);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    viewsCombo = new QComboBox;
    leftLayout->addWidget(viewsCombo, 1, Qt::AlignRight);
    leftLayout->addWidget(list);
    layout->addLayout(leftLayout, 0, 0);
    layout->addWidget(view, 0, 1);
    setLayout(layout);
    
    doDatabaseSelection(conn);

    Database database = conn.getDatabase(selectedDatabase.toStdString());
    
    getViews(database);
    
    connect(viewsCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(loadSelectedView(const QString &)));
}

 
void MainWindow::loadSelectedView(const QString &view){
    Model* m = new Model(conn);
    
    
    QString design = viewsCombo->itemData(viewsCombo->currentIndex()).toString();
    
    m->loadDocuments(selectedDatabase, design, view);
    list->setModel(m);
}
            
            
void MainWindow::getViews(Database& database){
    vector<Document> views = database.listViews();
    for (unsigned int i=0; i<views.size(); i++ ) {
        Document doc = views[i];
        Variant v = doc.getData();
        Object obj = boost::any_cast<Object>(*v);
        if ( obj.find("views") != obj.end() ) {
            Object views = boost::any_cast<Object>(*obj["views"]);
            Object::iterator it = views.begin();
            Object::iterator end = views.end();
            while( it != end ) {
                viewsCombo->addItem(QString(it->first.c_str()), QString(doc.getID().c_str()));
                it++;
            }
        }
        
    }
}

void MainWindow::databaseSelected(const QString &database){
    selectedDatabase = database;
    dlg->accept();
}

void MainWindow::doDatabaseSelection(Connection &conn){
    vector<string> dbs = conn.listDatabases();
    
    dlg = new QDialog;
    QFormLayout layout;
    
    QComboBox *combo = new QComboBox(dlg);
    for(unsigned int i=0; i<dbs.size(); i++){
        combo->insertItem(i, QString(dbs[i].c_str()));
    }
    
    connect(combo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(databaseSelected(const QString &)));
    
    layout.addRow(new QLabel("Select a database"), combo);
    dlg->setLayout(&layout);
    dlg->setFixedSize(350, 100);            
    dlg->exec();
    
    
}




int main(int argc, char *argv[])
{
    Application app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
