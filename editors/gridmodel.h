

#ifndef __GRIDMODEL_H
#define __GRIDMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QHeaderView>
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
private:
    QVariant var;
    QList<QVariant> list;
    QStringList headers;
    bool changed;
};














#endif
