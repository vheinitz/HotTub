
#include "gridmodel.h"
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QHeaderView>
#include <QDebug>


GridModel::GridModel(QVariant _var, QStringList _headers) : QAbstractTableModel()
{
    var = _var;
    headers = _headers;
    list = var.toList();

    changed = false;
}


QVariant GridModel::getVariant(){
    return QVariant(list);
}

void GridModel::setHeaders(QStringList _headers){
    /*headers = _headers;
    emit layoutChanged();*/
}

int GridModel::rowCount(const QModelIndex &index) const {
	Q_UNUSED(index);
    return list.size() + 1;
}

int GridModel::columnCount(const QModelIndex &index) const {
	Q_UNUSED(index);
    return headers.size();
}

bool GridModel::hasChanges(){
    return changed;
}


QVariant GridModel::data(const QModelIndex &index, int role) const {
	if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        if ( index.row() < list.size() ) {
            QVariant var = list[index.row()];
            QVariantMap map = var.toMap();
            if ( index.column() < map.values().size() ) {
                return map[headers[index.column()]];
            }
        }
    }
	return QVariant();
    
}

bool GridModel::setData(const QModelIndex& index, const QVariant& value, int role)  {
    if ( index.row() >= list.size() ) {
        QVariantMap map = QVariantMap();
        map[headers[index.column()]] = value;
        list << QVariant(map);
        changed = true;
		emit dataChanged();
        return true;
    } else {
        QVariant var = list[index.row()];
        QVariantMap map = var.toMap();
        map[headers[index.column()]] = value;
        list[index.row()] = QVariant(map);
        changed = true;
		emit dataChanged();
        return true;
    }
}


QVariant GridModel::headerData( int section, Qt::Orientation orientation, int role ) const {
	if ( role == Qt::DisplayRole ) {
        if ( orientation == Qt::Horizontal ) {
            return headers[section];
        }
	}
	return QVariant();
}


Qt::ItemFlags GridModel::flags( const QModelIndex &index ) const {
	Q_UNUSED( index );
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled; 
}

