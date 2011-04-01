
#ifndef __MODEL_H
#define __MODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QVariant>
#include <QStack>
#include <vector>
#include "qcouch/document.h"
#include "qcouch/qcouch.h"

using namespace std;

class QVariant;
class QAbstractTableModel;
class QAbstractItemModel;

class Model : public QAbstractTableModel {
	
	Q_OBJECT
	
public:
	Model(QString, QCouch &);
	
	int rowCount(const QModelIndex &) const;
	int columnCount(const QModelIndex &) const;
	QVariant data(const QModelIndex &, int) const;
	QVariant headerData( int, Qt::Orientation, int ) const; 
	Qt::ItemFlags flags( const QModelIndex & ) const;
	
    void loadView( QString database, QString design, QString view, QString startkey, QString endkey, bool descending, QString startKeyDocId="" );
    void nextPage( QString database, QString design, QString view, bool descending );
    void previousPage( QString database, QString design, QString view, bool descending );
    bool hasNextPage();
    bool hasPreviousPage();
    Document getDocument(int);

    void updateDocument(Document &doc);
    void removeDocument(Document &doc);
    void addDocument(Document &doc);

    QStringList getColumns(); 
    QStringList getHiddenColumns();
    void setColumns(QStringList);
private:
	int count;
    QList<QString> columns;
    QList<QString> hiddenColumns;
    
    QString database;
    QCouch &couch;
    
    QList<Document> documents;
    
    QString nextStartKey;
    QString nextStartKeyDocId;
    
    QString currentStartKey;
    QString currentStartKeyDocId;
    
    QStack<QPair<QString, QString> > previousStack;
};



#endif
