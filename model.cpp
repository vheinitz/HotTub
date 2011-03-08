
#include "model.h"
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include "qmessagebox.h"
#include <QModelIndex>
#include <QVariant>
#include <QHeaderView>
#include <QDebug>
#include <vector>
#include "qcouch/document.h"


using namespace std;

Model::Model(QString _database, QCouch& couch) : QAbstractTableModel(),
couch(couch)
{
    database = _database;
}



void Model::loadView( QString database, QString design, QString view ) {
    
    QList<QVariant> results = couch.getView(database, design, view);
    
    foreach(QVariant result, results ) {
        QVariantMap map = result.toMap();
        QString id = map["id"].toString();
        
        Document doc = couch.getDocument(database, id, "");
        documents.append(doc);
    }
    
    if ( documents.length() > 0 ) {
        Document doc = documents[0];
        QVariantMap fields = doc.getMap();
    
        foreach(QString key, fields.keys() ){
            if ( key != "_id" && key != "_rev" && key != "_attachments" ) {
                columns.append(key);
            }
        }
    }
    
}


int Model::rowCount(const QModelIndex &index) const {
	Q_UNUSED(index);
	return documents.size();
}

int Model::columnCount(const QModelIndex &index) const {
	Q_UNUSED(index);
    return columns.size();
}

Document Model::getDocument(int index){
    return documents[index];
}



QVariant Model::data(const QModelIndex &index, int role) const {
	if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
		if ( index.row() < documents.size() ) {
            Document doc = documents[index.row()];
            QVariantMap fields = doc.getMap();
            QString col = columns[index.column()];
            return fields[col];
            
        }
	}
	return QVariant();
    
}

QVariant Model::headerData( int section, Qt::Orientation orientation, int role ) const {
	if ( role == Qt::DisplayRole ) { 
		if ( orientation == Qt::Horizontal ) {
            return columns[section];
		} else {
			return QVariant(section+1);
		}
	}
	return QVariant();
}


Qt::ItemFlags Model::flags( const QModelIndex &index ) const {
	Q_UNUSED( index );
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled; 
     
}

void Model::updateDocument(Document &document){
    for( int i=0; i<documents.size(); i++){
        if ( documents[i].getId() == document.getId() ) {
            documents.replace(i, document);
            const QModelIndex topLeft = createIndex(i, 0);
            const QModelIndex bottomRight = createIndex(i, columns.size());
            emit dataChanged(topLeft, bottomRight);
            return;
        }
    }
    
}

void Model::removeDocument(Document &document){
    for( int i=0; i<documents.size(); i++){
        if ( documents[i].getId() == document.getId() ) {
            documents.removeAt(i);
            emit layoutChanged();
            return;
        }
    }
}

void Model::addDocument(Document &doc){
    documents.append(doc);
    emit layoutChanged();
}


