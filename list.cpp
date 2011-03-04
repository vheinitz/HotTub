
#include "list.h"


List::List(QWidget *parent) : QTableView(parent) {

}



void List::selectionChanged ( const QItemSelection &selected, const QItemSelection &deselected ) {
    Q_UNUSED(deselected);
    int index = selected.indexes().first().row();
    emit selectionChanged(index);
} 

