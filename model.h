
#ifndef __MODEL_H
#define __MODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QVariant>
#include <vector>
#include "qcouch/document.h"

using namespace std;

class QVariant;
class QAbstractTableModel;
class QAbstractItemModel;

class Model : public QAbstractTableModel {
	
	Q_OBJECT
	
public:
	Model();
	
	int rowCount(const QModelIndex &) const;
	int columnCount(const QModelIndex &) const;
	QVariant data(const QModelIndex &, int) const;
	QVariant headerData( int, Qt::Orientation, int ) const; 
	Qt::ItemFlags flags( const QModelIndex & ) const;
    Document getDocument( const int) const;
    void loadDocuments(const QString &, const QString&, const QString &);
	
private:
	int count;
	vector<Document> docs;
    vector<string> fields;
};



#endif