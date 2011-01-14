

#ifndef __ATTACHMENTS_H
#define __ATTACHMENTS_H

#include <QtGui>


class Attachments : public QWidget {

public:
    Attachments(QWidget *parent);
    QSize sizeHint();
    void paintEvent(QPaintEvent *event);
    void acceptUrl(QUrl url);
private:
    
    QList<QUrl> files;
  
    static const int iconWidth = 44;
    static const int iconHeight = 57;
};



#endif
