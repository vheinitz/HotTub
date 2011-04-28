/*
 This file is part of Hot Tub, a project to help interactions with Apache CouchDB
 Copyright 2011 Matt Jennings <jennmat@gmail.com>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3.0 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
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
	void resetPages();
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
