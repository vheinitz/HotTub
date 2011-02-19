
#include "list.h"

List::List(QWidget *parent) : QTableView(parent) {

    horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    horizontalHeader()->setStretchLastSection(true);

}