

#ifndef __LIST_H
#define __LIST_H

#include <QtGui>

class List : public QTableView {
    Q_OBJECT

public:
    List(QWidget *parent = 0);
    void selectionChanged ( const QItemSelection &, const QItemSelection & );
signals:
	void selectionChanged(int);

    
};















#endif
