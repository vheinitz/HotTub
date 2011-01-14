


#include <QtGui>
#include "attachments.h"


Attachments::Attachments(QWidget* parent) : QWidget(parent) {
    
}

QSize Attachments::sizeHint(){
    return QSize(20,20);
}


void Attachments::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(5, height()));
    linearGrad.setColorAt(0, Qt::lightGray);
    linearGrad.setColorAt(1, Qt::gray);
    
    QBrush brush(linearGrad);
    
    painter.fillRect(0, 1, width(), height(), brush);
    painter.drawLine(0,0,width(),0);
    
    int columnWidth = iconWidth * 2;
    int columnHeight = iconHeight * 1.5;
    int leftmargin = 10;
    int columns = width() / columnWidth;
    
    
    for (int i=0; i<files.length(); i++ ) {
        QString canonicalPath = files[i].toLocalFile();
        QString filename = canonicalPath.split("/").last();
      
        QIcon icon = QIcon("icons/file.png");
        
        icon.paint( &painter, QRect(i*columnWidth+leftmargin,0,columnWidth,columnHeight) );
        
        painter.drawText( i*columnWidth+leftmargin, columnHeight-20, columnWidth, 30,
                         Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap, filename );
    }
    
    QWidget::paintEvent(event);
    
    
}


void Attachments::acceptUrl(QUrl url) {
    files.append(url);
}