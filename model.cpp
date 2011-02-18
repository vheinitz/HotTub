
#include "model.h"
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include "qmessagebox.h"
#include <QModelIndex>
#include <QVariant>
#include <vector>

#include "couchdb/CouchDB.hpp"

using namespace CouchDB;
using namespace std;

Model::Model(Connection& conn) : QAbstractTableModel(), 
conn( conn )
{
    
}

void Model::loadDocuments(const QString& database, const QString& design, const QString &view){
    Database db = conn.getDatabase(database.toStdString());
    vector<Document> documents = db.listView(design.toStdString(), view.toStdString(), "", "");
    
    docs.clear();
    for (unsigned int i=0; i<documents.size(); i++) {
        docs.push_back(documents[i]);
    }
    count = docs.size();
	cache = new Variant[count];
    
    if ( count > 0 ) {
        Variant v = getDocumentData(0);
        Object obj = boost::any_cast<Object>(*v);
        
        Object::iterator it = obj.begin();
        Object::iterator end = obj.end();
        while( it != end ) {
            cout << it->first << endl;
            fields.push_back(it->first);
            it++;
        }
        
    }
    
    
}

int Model::rowCount(const QModelIndex &index) const {
	Q_UNUSED(index);
	return count+3;
}

int Model::columnCount(const QModelIndex &index) const {
	Q_UNUSED(index);
    cout << fields.size() << endl;
	return fields.size();
}


Variant Model::getDocumentData(const int index) const {
	Variant v;
	Document doc = docs[index];
	if ( cache[index] == NULL ){
		v = doc.getData();
		cache[index] = v;
	} else {
		v = cache[index];
	}
    return v;
}

QVariant Model::data(const QModelIndex &index, int role) const {
	if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
		if ( (unsigned int)index.row() < docs.size() ) {
			Variant v = getDocumentData(index.row());
			Object obj = boost::any_cast<Object>(*v);
			
            string field = fields[index.column()];
            
            if ( obj.find(field) == obj.end() ) {
                return QVariant();
            }
            try { 
                string s = boost::any_cast<string>(*obj[field]);
                return QVariant(s.c_str());
            }catch(boost::bad_any_cast& e){
                
            }
        }
	}
	return QVariant();
    
}

QVariant Model::headerData( int section, Qt::Orientation orientation, int role ) const {
	if ( role == Qt::DisplayRole ) { 
		if ( orientation == Qt::Horizontal ) {
            return QString(fields[section].c_str());
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
