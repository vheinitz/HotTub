
#include <QApplication>
#include "view.h"

#include <QtGui>
#include "list.h"
#include "view.h"
#include "main.h"
#include "model.h"
#include "app.h"
#include "listconfig.h"
#include "qcouch/qcouch.h"


using namespace std;


MainWindow::MainWindow() : QMainWindow() {
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

    QPushButton *configureViewButton = new QPushButton(tr("Configure"));
    connect(configureViewButton, SIGNAL(clicked()), this, SLOT(configureViewColumns()));
    
    nextPageButton = new QPushButton(tr("Next Page"));
    connect(nextPageButton, SIGNAL(clicked()), this, SLOT(nextPage()));
    
    previousPageButton = new QPushButton(tr("Previous Page"));
    connect(previousPageButton, SIGNAL(clicked()), this, SLOT(previousPage()));
    
    QHBoxLayout *viewSouthLayout = new QHBoxLayout;
    viewSouthLayout->addWidget(configureViewButton);    
    viewSouthLayout->addStretch();
    viewSouthLayout->addWidget(previousPageButton);
    viewSouthLayout->addWidget(nextPageButton);
    
    leftLayout->addLayout(viewSouthLayout, 0);

    layout->addLayout(leftLayout, 0, 0);
    layout->addWidget(view, 0, 1);
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);
    
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

    previousPageButton->setDisabled(true);
}

void MainWindow::nextPage(){
    bool desc = descending->checkState() == Qt::Checked;
    model->nextPage( selectedDatabase, design, currentView, desc);
    previousPageButton->setDisabled(false);
    if( !model->hasNextPage()){
        nextPageButton->setDisabled(true);
    }
}

void MainWindow::previousPage(){
    bool desc = descending->checkState() == Qt::Checked;
    model->previousPage( selectedDatabase, design, currentView, desc);
    if ( !model->hasPreviousPage() ) {
        previousPageButton->setDisabled(true);
    }
}


void MainWindow::updateView(){
    QString startkey = startKeyEdit->text();
    QString endkey = endKeyEdit->text();
    bool desc = descending->checkState() == Qt::Checked;
    model->loadView( selectedDatabase, design, currentView, startkey, endkey, desc);
    loadViewConfiguration();
 
    if( !model->hasNextPage()){
        nextPageButton->setDisabled(true);
    } else {
        nextPageButton->setDisabled(false);
    }
    
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

void MainWindow::loadViewConfiguration(){
     
}

void MainWindow::loadSelectedView(const QString& selectedView){
    if ( view->hasChanges() ) {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes before continuing?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        
        switch( ret ) {
            case QMessageBox::Yes:
                view->saveChanges();
                break;
            case QMessageBox::No:
            default: // just for sanity
                break;
           
        }
    }
    
    
    QVariant var = viewsCombo->itemData(viewsCombo->currentIndex());
    QVariantMap map = var.toMap();
    design = map["design"].toString();
    currentView = map["view"].toString();
    
    model = new Model(selectedDatabase, couch);
    QString startkey = startKeyEdit->text();
    QString endkey = endKeyEdit->text();
    bool desc = descending->checkState() == Qt::Checked;
    model->loadView( selectedDatabase, design, currentView, startkey, endkey, desc );
    loadViewConfiguration();
    
    view->clear(); 
    
    view->loadTemplate(design, currentView);
    
    list->setModel(model);
    list->resizeColumnsToContents();
    
    if( !model->hasNextPage()){
        nextPageButton->setDisabled(true);
    } else {
        nextPageButton->setDisabled(false);
    }
    
}
            
            
void MainWindow::getViews(QString& db){
    QList<QVariant> views = couch.listViews(db);
    foreach(QVariant view, views){
        QVariantMap map = view.toMap();
        QVariantMap doc = map["doc"].toMap();
        QString design = doc["_id"].toString();
        QString designName = design.split("/")[1];
        QVariantMap views = doc["views"].toMap();
        foreach(QString key, views.keys() ){
            QVariantMap data;
            data["design"] = design;
            data["view" ] = key;
            viewsCombo->addItem(designName+": "+key, QVariant(data) );
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


void MainWindow::configureViewColumns(){
	viewConfig = new ViewConfig;

	connect(viewConfig, SIGNAL(accepted()), this, SLOT(columnConfigAccepted()));	
	viewConfig->show();
	QStringList headers = model->getColumns();
	
	foreach(QString header, headers){
		viewConfig->addCurrentColumn(header);
	}
    
    QStringList hiddenColumns = model->getHiddenColumns();
    foreach(QString hiddenColumn, hiddenColumns){
        viewConfig->addColumn(hiddenColumn);
    }
}


void MainWindow::columnConfigAccepted(){
	QStringList columns = viewConfig->getCurrentColumns();

    QVariantMap map;
    map["columns"] = QVariant(columns);
    
    try {
        Document doc = couch.getDocument(selectedDatabase, "templates/"+design+"/_list/"+currentView);
        couch.updateDocument(selectedDatabase, doc.getId(), doc.getRevision(), QVariant(map));
    }catch (DocumentNotFoundException) {
        couch.createDocument(selectedDatabase, "templates/"+design+"/_list/"+currentView, QVariant(map));
    }
    
	model->setColumns(columns);	
    list->resizeColumnsToContents();

}

void MainWindow::closeEvent(QCloseEvent *event){
    if ( view->hasChanges() ) {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        
        switch( ret ) {
            case QMessageBox::Save:
                view->saveChanges();
                event->accept();
                break;
            case QMessageBox::Cancel:
            default: // just for sanity
                event->ignore();
                break;
            case QMessageBox::Discard:
                event->accept();
                break;
        }
    } else {
        event->accept();
    }
}



int main(int argc, char *argv[])
{
    Application app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
