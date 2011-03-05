
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

void Model::setDocuments(QList<QVariant> _documents){
    documents = _documents;
    
    if (documents.size() > 0 ){
        QVariant var = documents[0];
        QVariantMap map = var.toMap();
        QString id = map["id"].toString();
        
        Document doc = couch.getDocument(database, id, "");
        QVariantMap fields = doc.getMap();
        
        foreach(QString key, fields.keys() ){
            if ( key != "_id" && key != "_rev" ) {
                columns.append(key);
            }
        }
        
    }
}

//void Model::loadDocuments(const QString& database, const QString& design, const QString &view){
    /*Database db = conn.getDatabase(database.toStdString());
    vector<Document> documents = db.listView(design.toStdString(), view.toStdString(), "", "");
    
    docs.clear();
    for (unsigned int i=0; i<documents.size(); i++) {
        docs.push_back(documents[i]);
    }
    count = docs.size();
	cache = new Variant[count];
    
    if ( count > 0 ) {
        Object obj;
        Document doc = documents[0];
        boost::any value = doc.getValue();
        cout << "Got value" << endl;
        const type_info &type = value.type();
        cout << "Got value type" << endl;
        if ( type == typeid(Object) ){
            obj = boost::any_cast<Object>(value);
        } else {
            Variant v = getDocumentData(0);
            Object obj = boost::any_cast<Object>(*v);
        }
        
        Object::iterator it = obj.begin();
        Object::iterator end = obj.end();
        while( it != end ) {
            if ( it->first.compare("_id") != 0 && it->first.compare("_rev") != 0 && it->first.compare("_attachments") != 0 ) {
                fields.push_back(it->first);
            }
            it++;
        }
    }*/
//}

int Model::rowCount(const QModelIndex &index) const {
	Q_UNUSED(index);
	return documents.size();
}

int Model::columnCount(const QModelIndex &index) const {
	Q_UNUSED(index);
    return columns.size();
}

/*
void Model::getDocumentData(const int index) const {
	Variant v;
	Document doc = docs[index];
	if ( cache[index] == NULL ){
		v = doc.getData();
		cache[index] = v;
	} else {
		v = cache[index];
	}
    return v;
}*/

QVariant Model::data(const QModelIndex &index, int role) const {
	if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
		if ( index.row() < documents.size() ) {
            QVariant var = documents[index.row()];
            QVariantMap map = var.toMap();
            QString id = map["id"].toString();
            
            Document doc = couch.getDocument(database, id, "");
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
	/*Q_UNUSED( index );
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled; 
     */
}
