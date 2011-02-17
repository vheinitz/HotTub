
#ifndef __MODEL_H
#define __MODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QVariant>
#include <vector>
#include "couchdb/CouchDB.hpp"

using namespace CouchDB;
using namespace std;

class QVariant;
class QAbstractTableModel;
class QAbstractItemModel;

class Model : public QAbstractTableModel {
	
	Q_OBJECT
	
public:
	Model(Connection &conn);
	
	int rowCount(const QModelIndex &) const;
	int columnCount(const QModelIndex &) const;
	QVariant data(const QModelIndex &, int) const;
	QVariant headerData( int, Qt::Orientation, int ) const; 
	Qt::ItemFlags flags( const QModelIndex & ) const;
    Document getDocument( const int) const;
    void loadDocuments(const QString &, const QString&, const QString &);
	
private:
	Variant getDocumentData(const int) const;
    int count;
	Connection &conn;
	vector<Document> docs;
	Variant *cache;
};



#endif