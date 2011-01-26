


#include <QtGui>
#include "attachments.h"
#include <iostream>

using namespace std;


Attachments::Attachments(QWidget* parent) : QWidget(parent) {
    selectedColumn = -1;
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
    
    columnWidth = iconWidth * 2;
    int columnHeight = iconHeight * 1.5;
    leftmargin = 10;
    columns = width() / columnWidth;
    
    
    for (int i=0; i<files.length(); i++ ) {
        QString canonicalPath = files[i].toLocalFile();
        QString filename = canonicalPath.split("/").last();
      
        QIcon icon = QIcon("icons/file.png");
    
        QRect fileIconRect = QRect(i*columnWidth+leftmargin,0,columnWidth,columnHeight);
        if ( i == selectedColumn ) {
            painter.setPen(QPen(Qt::white));
            icon.paint(&painter, fileIconRect, Qt::AlignCenter, QIcon::Selected );            
            QRect boundingRect;
            
            painter.drawText( i*columnWidth+leftmargin, columnHeight-10, columnWidth, 30,
                             Qt::AlignHCenter | Qt::TextWordWrap, filename, &boundingRect );
            
            boundingRect.setLeft(boundingRect.left()-4);
            boundingRect.setRight(boundingRect.right()+4);
            QPainterPath path;
            path.addRoundedRect(boundingRect, 10, 10);
            painter.fillPath(path, QBrush(QColor(46,115,218)));
            
            painter.drawText( i*columnWidth+leftmargin, columnHeight-10, columnWidth, 30,
                             Qt::AlignHCenter | Qt::TextWordWrap, filename, &boundingRect );
            
            
        } else {   
            icon.paint( &painter, fileIconRect );
            painter.setPen(QPen(Qt::black));
            painter.drawText( i*columnWidth+leftmargin, columnHeight-10, columnWidth, 30,
                             Qt::AlignHCenter | Qt::TextWordWrap, filename );
            
        }
    }
    
    QWidget::paintEvent(event);
    
    
}


void Attachments::acceptUrl(QUrl url) {
    files.append(url);
}


void Attachments::mousePressEvent(QMouseEvent *event){
    int col = event->x() / columnWidth;
    selectedColumn = col;
    update();
}


void Attachments::mouseDoubleClickEvent(QMouseEvent *event){
    int col = event->x() / columnWidth;
        
    if ( col < files.length() ) {
        
    }
}

