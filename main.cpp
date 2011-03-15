
#include <QApplication>
#include "view.h"

#include <QtGui>
#include "list.h"
#include "view.h"
#include "main.h"
#include "model.h"
#include "app.h"

#include "qcouch/qcouch.h"


using namespace std;


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setMinimumSize(1200, 600);
    
    view = new View(couch, this);
    list = new List(this);
    
    view->setMinimumWidth(600);
    list->setMinimumWidth(600);
    
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(5,5,5,5);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    viewsCombo = new QComboBox;

    startKeyEdit = new QLineEdit;
    endKeyEdit = new QLineEdit;
    QLabel *startLabel = new QLabel("Start Key:");
    QLabel *endLabel = new QLabel("End Key:");
	descending = new QCheckBox("Desc", this);
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setSpacing(5);
    topLayout->addWidget(viewsCombo);
    topLayout->addStretch();
	topLayout->addWidget(startLabel);
    topLayout->addWidget(startKeyEdit);
    topLayout->addWidget(endLabel);
    topLayout->addWidget(endKeyEdit);
    topLayout->addWidget(descending);
    leftLayout->setSpacing(5);
    leftLayout->addLayout(topLayout, 0);

    leftLayout->addWidget(list, 1);
    layout->addLayout(leftLayout, 0, 0);
    layout->addWidget(view, 0, 1);
    setLayout(layout);
    
    couch.connect("http://localhost", 5984);
    
    doDatabaseSelection();

    view->setDatabase(selectedDatabase);
    getViews(selectedDatabase);
    
    connect(viewsCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(loadSelectedView(const QString &)));
    connect(list, SIGNAL(selectionChanged(int)), this, SLOT(listSelectionChanged(int)));
    
    connect(view, SIGNAL(documentUpdated(Document &)), this, SLOT(documentUpdated(Document &)));
    connect(view, SIGNAL(documentDeleted(Document &)), this, SLOT(documentDeleted(Document &)));
    connect(view, SIGNAL(documentAdded(Document &)), this, SLOT(documentAdded(Document &)));

    connect(descending, SIGNAL(stateChanged(int)), this, SLOT(descendingCheckStateChanged(int)));

    keyTimer.setSingleShot(true);
    connect(&keyTimer, SIGNAL(timeout()), this, SLOT(updateView()));

    connect(startKeyEdit, SIGNAL(textChanged(const QString &)), this, SLOT(startKeyChanged(const QString &)));
    connect(endKeyEdit, SIGNAL(textChanged(const QString &)), this, SLOT(endKeyChanged(const QString &)));

}


void MainWindow::updateView(){
    QString startkey = startKeyEdit->text();
    QString endkey = endKeyEdit->text();
    bool desc = descending->checkState() == Qt::Checked;
    model->loadView( selectedDatabase, design, currentView, startkey, endkey, desc);
    list->resizeColumnsToContents();
}

void MainWindow::startKeyChanged(const QString& text){
    Q_UNUSED(text);
    keyTimer.stop();
    keyTimer.start(500);
}

void MainWindow::descendingCheckStateChanged(int state){
    Q_UNUSED(state);
    updateView();
}

void MainWindow::endKeyChanged(const QString& text) {
    Q_UNUSED(text);
    keyTimer.stop();
    keyTimer.start(500);
}

void MainWindow::documentUpdated(Document& doc){
    model->updateDocument(doc);
}

void MainWindow::documentDeleted(Document& doc){
    model->removeDocument(doc);
}

void MainWindow::documentAdded(Document& doc){
    model->addDocument(doc);
}

void MainWindow::listSelectionChanged(int index){
    if( index >= 0 ){
      Document doc = model->getDocument(index);
      view->loadDocument(doc);
    }
    
}

void MainWindow::loadSelectedView(const QString& selectedView){
    design = viewsCombo->itemData(viewsCombo->currentIndex()).toString();
    currentView = selectedView;
    
    model = new Model(selectedDatabase, couch);
    QString startkey = startKeyEdit->text();
    QString endkey = endKeyEdit->text();
    bool desc = descending->checkState() == Qt::Checked;
    model->loadView( selectedDatabase, design, selectedView, startkey, endkey, desc );
    view->clear(); 
    view->setDesign(design);
    view->setView(selectedView);

    list->setModel(model);
    list->resizeColumnsToContents();
}
            
            
void MainWindow::getViews(QString& db){
    QList<QVariant> views = couch.listViews(db);
    foreach(QVariant view, views){
        QVariantMap map = view.toMap();
        QVariantMap doc = map["doc"].toMap();
        QVariantMap views = doc["views"].toMap();
        foreach(QString key, views.keys() ){
            viewsCombo->addItem(key, map["id"]);
        }
    }
    viewsCombo->setCurrentIndex(-1);
}

void MainWindow::databaseSelected(const QString &database){
    selectedDatabase = database;
    dlg->accept();
}

void MainWindow::doDatabaseSelection(){
    QList<QString> dbs = couch.listDatabases();
    
    dlg = new QDialog;
    QFormLayout layout;
    
    QComboBox *combo = new QComboBox(dlg);
    foreach(QString str, dbs){
        combo->addItem(str);
    }
    combo->setCurrentIndex(-1);
    
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
