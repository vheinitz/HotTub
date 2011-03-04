
#include "model.h"
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include "qmessagebox.h"
#include <QModelIndex>
#include <QVariant>
#include <QHeaderView>
#include <vector>
#include "qcouch/document.h"


using namespace std;

Model::Model() : QAbstractTableModel() 
{
       
}

void Model::loadDocuments(const QString& database, const QString& design, const QString &view){
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
}

int Model::rowCount(const QModelIndex &index) const {
	Q_UNUSED(index);
	return count;
}

int Model::columnCount(const QModelIndex &index) const {
	Q_UNUSED(index);
    //return fields.size();
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
	/*if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
		if ( (unsigned int)index.row() < docs.size() ) {
            Object obj;
            boost::any value = docs[index.row()].getValue();
            const type_info &type = value.type();
            
            if ( type == typeid(Object) ){
                obj = boost::any_cast<Object>(value);
            } else {
                Variant v = getDocumentData(index.row());
                Object obj = boost::any_cast<Object>(*v);
			}
                
            string field = fields[index.column()];
            
            if ( obj.find(field) == obj.end() ) {
                return QVariant();
            }
            boost::any val = *obj[field];
            const type_info &t = val.type();
            if(t == typeid(string)) {
                string s = boost::any_cast<string>(val);
                return QVariant(s.c_str());
            } else if(t == typeid(bool))
                return QVariant(boost::any_cast<bool>(val));
            else if(t == typeid(int))
                return QVariant(boost::any_cast<int>(val));
            else if(t == typeid(double))
                return QVariant(boost::any_cast<double>(val));
            
        }
	}*/
	return QVariant();
    
}

QVariant Model::headerData( int section, Qt::Orientation orientation, int role ) const {
	/*if ( role == Qt::DisplayRole ) { 
		if ( orientation == Qt::Horizontal ) {
            return QString(fields[section].c_str());
		} else {
			return QVariant(section+1);
		}
	}
	return QVariant();*/
}


Qt::ItemFlags Model::flags( const QModelIndex &index ) const {
	/*Q_UNUSED( index );
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled; 
     */
}
