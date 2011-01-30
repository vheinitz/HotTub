


#include <QtGui>
#include "attachments.h"
#include <iostream>

using namespace std;


Attachments::Attachments(QWidget* parent) : QWidget(parent) {
    selectedColumn = -1;
    
    contextMenu = new QMenu();
    QAction *action = new QAction("New Folder", contextMenu);   
    connect(action, SIGNAL(triggered()), this, SLOT(addFolder()));
    contextMenu->addAction(action);

    columnWidth = iconWidth * 2;
    columnHeight = iconHeight * 1.5;
    
    setAcceptDrops(true);
}

QSize Attachments::sizeHint(){
    return QSize(20,20);
}

void Attachments::dragEnterEvent(QDragEnterEvent *event){
    event->acceptProposedAction();
}

void Attachments::dragMoveEvent(QDragMoveEvent *event){
    selectedColumn = -1;
    int col = event->pos().x() / columnWidth;
    if ( col < files.length() ) {
        if ( files[col].type == FOLDER ) {
            selectedColumn = col;
        }
    }

    update();
    
}

void Attachments::dropEvent(QDropEvent *event){
    if ( selectedColumn > -1 && selectedColumn < files.length() ) {
        if ( files[selectedColumn].type == FOLDER ) {
            event->acceptProposedAction();
            return;
        }
    }
    
    
    if ( event->mimeData()->hasUrls() ) {
        event->acceptProposedAction();
        QList<QUrl> urls = event->mimeData()->urls();
        for(int i=0; i<urls.length(); i++){
            acceptUrl(urls[i]);
        }
    }
    
    event->setDropAction(Qt::IgnoreAction);
    event->accept();
}


void paintAttachedObject(AttachedObject obj, QPainter& painter, QRect rect, bool selected){
    
    QString filename = obj.name;
    QIcon *icon;
    if ( obj.type == ATTACHMENT ) {
        icon = new QIcon("icons/file.png");
    } else {
        icon = new QIcon("icons/folder.png");
    }
    
    QRect textRect = QRect(rect.x(), rect.height()-10, rect.width(), 30);
    
    
    if ( selected == true ) {
        painter.setPen(QPen(Qt::white));
        icon->paint(&painter, rect, Qt::AlignCenter, QIcon::Selected );            
        QRect boundingRect;
        
        
        painter.drawText( textRect, Qt::AlignHCenter | Qt::TextWordWrap, filename, &boundingRect );
        
        boundingRect.setLeft(boundingRect.left()-5);
        boundingRect.setRight(boundingRect.right()+5);
        QPainterPath path;
        path.addRoundedRect(boundingRect, 10, 10);
        painter.fillPath(path, QBrush(QColor(46,115,218)));
        
        painter.drawText( textRect, Qt::AlignHCenter | Qt::TextWordWrap, filename, &boundingRect );
        
        
    } else {   
        icon->paint( &painter, rect );
        painter.setPen(QPen(Qt::black));
        painter.drawText( textRect, Qt::AlignHCenter | Qt::TextWordWrap, filename );
        
    }
    
}

void Attachments::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    
    painter.setRenderHints(QPainter::Antialiasing);
    
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(5, height()));
    linearGrad.setColorAt(0, Qt::lightGray);
    linearGrad.setColorAt(1, Qt::gray);
    
    QBrush brush(linearGrad);
    
    painter.fillRect(0, 1, width(), height(), brush);
    painter.drawLine(0,0,width(),0);
    
    leftmargin = 10;
    columns = width() / columnWidth;
    
    
    for (int i=0; i<files.length(); i++ ) {
             
        /*QString filename = files[i].name;
        QIcon *icon;
        if ( files[i].type == ATTACHMENT ) {
            icon = new QIcon("icons/file.png");
        } else {
            icon = new QIcon("icons/folder.png");
        }
        
        QRect fileIconRect = QRect(i*columnWidth+leftmargin,0,columnWidth,columnHeight);
        if ( i == selectedColumn ) {
            painter.setPen(QPen(Qt::white));
            icon->paint(&painter, fileIconRect, Qt::AlignCenter, QIcon::Selected );            
            QRect boundingRect;
            
            painter.drawText( i*columnWidth+leftmargin, columnHeight-10, columnWidth, 30,
                             Qt::AlignHCenter | Qt::TextWordWrap, filename, &boundingRect );
            
            boundingRect.setLeft(boundingRect.left()-5);
            boundingRect.setRight(boundingRect.right()+5);
            QPainterPath path;
            path.addRoundedRect(boundingRect, 10, 10);
            painter.fillPath(path, QBrush(QColor(46,115,218)));
            
            painter.drawText( i*columnWidth+leftmargin, columnHeight-10, columnWidth, 30,
                             Qt::AlignHCenter | Qt::TextWordWrap, filename, &boundingRect );
            
            
        } else {   
            icon->paint( &painter, fileIconRect );
            painter.setPen(QPen(Qt::black));
            painter.drawText( i*columnWidth+leftmargin, columnHeight-10, columnWidth, 30,
                             Qt::AlignHCenter | Qt::TextWordWrap, filename );
            
        }*/
        QRect fileIconRect = QRect(i*columnWidth+leftmargin,0,columnWidth,columnHeight);
        paintAttachedObject(files[i], painter, fileIconRect, i == selectedColumn);
    }
    
    QWidget::paintEvent(event);
    
    
}


void Attachments::acceptUrl(QUrl url) {
    
    AttachedObject obj;
    QString canonicalPath = url.toLocalFile();
    QString filename = canonicalPath.split("/").last();
    obj.name = filename;
    obj.type = ATTACHMENT;
    files.append(obj);
    
    update();
}


void Attachments::mousePressEvent(QMouseEvent *event){
    int col = event->x() / columnWidth;
    selectedColumn = col;
    update();
    
    if ( event->button() == Qt::LeftButton ) {
        dragStartPosition = event->pos();
    }
}


void Attachments::mouseDoubleClickEvent(QMouseEvent *event){
    int col = event->x() / columnWidth;
        
    if ( col < files.length() ) {
        
    }
}


void Attachments::mouseMoveEvent(QMouseEvent *event){
    
    
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
        < QApplication::startDragDistance())
        return;
    
    if ( selectedColumn == -1 || selectedColumn >= files.length() ) return;
    
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    QIcon icon = QIcon("icons/file.png");
    drag->setMimeData(mimeData);
    
    QPixmap map(columnWidth+30,columnHeight+10);
    QPainter painter(&map);
    painter.begin(&map);
    painter.setOpacity(0.45);
    
    int i = selectedColumn;
    
    paintAttachedObject(files[i], painter, QRect(15,0,columnWidth+15,columnHeight), true);
    
    drag->setPixmap(map);
    drag->setHotSpot(QPoint(dragStartPosition.x()-selectedColumn*columnWidth+15-leftmargin ,dragStartPosition.y()));
    
    Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
    if ( dropAction == Qt::MoveAction ) {
        files.removeAt(i);
        selectedColumn = -1;
        update();
    }
}


void Attachments::contextMenuEvent(QContextMenuEvent *event){
    QPoint pos = mapToGlobal(event->pos());
    contextMenu->popup(pos);
}


void Attachments::addFolder(){
    bool ok;
    QString folderName = QInputDialog::getText(this, "Folder Name", "Enter new folder name", QLineEdit::Normal, QString(), &ok);
    if ( ok ) {
        AttachedObject obj;
        obj.name=folderName;
        obj.type=FOLDER;
        files.append(obj);
    
        update();
    }
}








