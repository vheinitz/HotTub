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

#ifndef __GRIDMODEL_H
#define __GRIDMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QHeaderView>
#include <QStringList>
#include <QDebug>

class GridModel : public QAbstractTableModel {
    Q_OBJECT
public:
	GridModel(QVariant var, QStringList headers);
	
	int rowCount(const QModelIndex &) const;
	int columnCount(const QModelIndex &) const;
	QVariant data(const QModelIndex &, int) const;
	QVariant headerData( int, Qt::Orientation, int ) const; 
	Qt::ItemFlags flags( const QModelIndex & ) const;
	bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole);
    
    bool hasChanges();
    QVariant getVariant();
    void setHeaders(QStringList);
signals:
	void dataChanged();
private:
    QVariant var;
    QList<QVariant> list;
    QStringList headers;
    bool changed;
};














#endif
