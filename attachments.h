

#ifndef __ATTACHMENTS_H
#define __ATTACHMENTS_H

#include <QtGui>


class Attachments : public QWidget {

public:
    Attachments(QWidget *parent);
    QSize sizeHint();
    void paintEvent(QPaintEvent *event);
    void acceptUrl(QUrl url);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    
    QList<QUrl> files;
  
    static const int iconWidth = 44;
    static const int iconHeight = 57;
    
    int columnWidth;
    int rowHeight;
    int columns;
    int leftmargin;
    int selectedColumn;
};



#endif
