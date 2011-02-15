
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
    loadDocuments();
    
}

void Model::loadDocuments(){
    Database database = conn.getDatabase("addresses");
    vector<Document> documents = database.listView("example4", "baz", "", "");
    
    docs.clear();
    for (unsigned int i=0; i<documents.size(); i++) {
        docs.push_back(documents[i]);
    }
    count = docs.size();
	cache = new Variant[count];
    
}

int Model::rowCount(const QModelIndex &index) const {
	Q_UNUSED(index);
	return count+3;
}

int Model::columnCount(const QModelIndex &index) const {
	Q_UNUSED(index);
	return 2;
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
			
			if (index.column() == 0 ) {
                if ( obj.find("name") == obj.end() ) {
                    return QVariant();
                }
				string s = boost::any_cast<string>(*obj["name"]);
				return QVariant(s.c_str());
			} else if ( index.column() == 1 ) {
				if ( obj.find("address") == obj.end() ) {
                    return QVariant();
                }
                string s = boost::any_cast<string>(*obj["address"]);
				return QVariant(s.c_str());	
			}
		}
	}
	return QVariant();
    
}

QVariant Model::headerData( int section, Qt::Orientation orientation, int role ) const {
	if ( role == Qt::DisplayRole ) { 
		if ( orientation == Qt::Horizontal ) {
			if ( section == 0 ) return QVariant("Name");
			if ( section == 1 ) return QVariant("Address");
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
