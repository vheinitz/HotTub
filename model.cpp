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


QStringList Model::getColumns(){
	return columns;
}

QStringList Model::getHiddenColumns(){
    return hiddenColumns;
}

void Model::setColumns(QStringList _columns){
	columns = _columns;	
	emit layoutChanged();
}

void Model::loadView( QString database, QString design, QString view, QString startkey, QString endkey, bool descending, QString startKeyDocId ) {
    
    QVariant startKeyVariant;
    QVariant endKeyVariant;
    if( startkey.size() > 0 ) {
       startKeyVariant = QVariant(startkey);
    }
    if( endkey.size() > 0 ){
       endKeyVariant = QVariant(endkey);
    }
   
    documents.clear();
    columns.clear();
    hiddenColumns.clear();
    QList<QVariant> results = couch.getView(database, design, view, startKeyVariant, endKeyVariant, descending, 26, true, startKeyDocId);
    
    for(int i=0; i<results.length() && i<25; i++){
        QVariant result = results[i];
        QVariantMap map = result.toMap();
        QString id = map["id"].toString();
        Document doc = Document(database, map["doc"]);
        documents.append(doc);
    }
    
    QSet<QString> potentialColumns;
    foreach(Document doc, documents){
        QVariantMap fields = doc.getMap();
        
        foreach(QString key, fields.keys() ){
            if ( key != "_id" && key != "_rev" && key != "_attachments" ) {
                QVariant val = fields[key];
                if ( val.canConvert(QVariant::Int) ||  
                     val.canConvert(QVariant::String) ||
                     val.canConvert(QVariant::Bool) ) {
                    potentialColumns.insert(key);
                }
            }
        }
    }
    
    
    try {
        /* If there is a saved preference for what columns to show, use that */
        Document doc = couch.getDocument(database, "templates/"+design+"/_list/"+view);
        QVariantMap map = doc.getMap();
        if ( map.find("columns") != map.end() ) {
            columns = map["columns"].toStringList();
            /* Everything not shown is hidden */
            foreach(const QString potentialColumn, potentialColumns ) {
                if ( !columns.contains(potentialColumn) ) {
                    hiddenColumns << potentialColumn;
                }
            }
        }
    }catch( DocumentNotFoundException ){
        /* Go through the documents looking for potential fields to include */
        if ( documents.length() > 0 ) {
            Document doc = documents[0];
            QVariantMap fields = doc.getMap();
            foreach(QString key, fields.keys() ){
                if ( key != "_id" && key != "_rev" && key != "_attachments" ) {
                    QVariant val = fields[key];
                    if ( val.canConvert(QVariant::Int) ||  
                        val.canConvert(QVariant::String) ||
                        val.canConvert(QVariant::Bool) ) {
                        columns.append(key);
                    }
                }
            }
        }    
    } 
    
    /* Store the nextStartKey and the nextStartKeyDocId for nextPage */
    if( results.length() == 26 ) {
        QVariant var = results[25];
        QVariantMap map = var.toMap();
        if ( map.find("key") != map.end() && map.find("id") != map.end() ) {
            nextStartKey = map["key"].toString();
            nextStartKeyDocId = map["id"].toString();
        }
        var = results[0];
        map = var.toMap();
        if ( map.find("key") != map.end() && map.find("id") != map.end() ) {
            currentStartKey = map["key"].toString();
            currentStartKeyDocId = map["id"].toString();
            qDebug() << previousStack;
        }
        
    } else {
        nextStartKey = "";
        nextStartKeyDocId = "";
    }
    
    
    emit layoutChanged();
}

bool Model::hasNextPage(){
    if ( nextStartKey.length() > 0 ){
        return true;
    }
    return false;
}

bool Model::hasPreviousPage(){
    return !previousStack.isEmpty();
}

void Model::resetPages(){
	previousStack.clear();
}

void Model::nextPage(QString database, QString design, QString view, bool descending ){
    if ( hasNextPage() ) {
        previousStack.push(qMakePair(currentStartKey, currentStartKeyDocId));
        loadView(database, design, view, nextStartKey, "", descending, nextStartKeyDocId);
    }
}

void Model::previousPage(QString database, QString design, QString view, bool descending ){
    if ( hasPreviousPage() ) {
        QPair<QString, QString> pair = previousStack.pop();
        loadView(database, design, view, pair.first, "", descending, pair.second);
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


