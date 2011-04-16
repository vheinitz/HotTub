


#include <QtGui>
#include "attachments.h"
#include <iostream>
#include "qcouch/document.h"

using namespace std;


Attachments::Attachments(QCouch& couch, QWidget* parent) : QWidget(parent), couch(couch) {
    selectedColumn = -1;
    
    contextMenu = new QMenu();
    newFolderAction = new QAction("New Folder", contextMenu);   
    connect(newFolderAction, SIGNAL(triggered()), this, SLOT(addFolder()));
    
    openAction = new QAction("Open", contextMenu);
    deleteAction = new QAction("Delete", contextMenu);
    
    openAction->setDisabled(true);
    deleteAction->setDisabled(true);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openObject()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteObject()));
    
    
    
    contextMenu->addAction(openAction);
    contextMenu->addAction(deleteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(newFolderAction);
    
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
    obj.url  = url;
    files.append(obj);
    
    emit fileAttached(url);
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
        AttachedObject obj = files[col];
        QDesktopServices::openUrl(QUrl("file://"+obj.downloadFileName));
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
    
    QPixmap map(columnWidth+30,columnHeight+10);
    QPainter painter(&map);
    painter.setOpacity(0.45);
    
    int i = selectedColumn;
    
    paintAttachedObject(files[i], painter, QRect(15,0,columnWidth+15,columnHeight), true);
    
    
    QList<QUrl> urls;
    urls.append(files[i].url);
    QString str = files[i].url.toString();
    cout << str.toStdString() << endl;
    mimeData->setUrls(urls);
    drag->setMimeData(mimeData);
    
    
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
    
    if ( selectedColumn > -1 && selectedColumn < files.length() ) {
        openAction->setDisabled(false);
        deleteAction->setDisabled(false);
    } else {
        openAction->setDisabled(true);
        deleteAction->setDisabled(true);
    }
    
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

bool Attachments::hasSelectedObject(){
    return selectedColumn > -1 && selectedColumn < files.length();
}

void Attachments::deleteObject(){
    if ( hasSelectedObject() ) {
        
        files.removeAt(selectedColumn);
        update();
    }
}

void Attachments::openObject(){
    
}



void Attachments::loadDocument(Document &doc){
    files.clear();
    QList<QString> attachments = doc.getAttachmentIds();
    foreach(QString attachment, attachments ) {
        AttachedObject obj;
        obj.name = attachment;
        obj.type = ATTACHMENT;
        obj.downloadFileName = couch.getAttachment(doc.getSourceDatabase(), doc.getId(), attachment);
        
        files.append(obj);
        
        
    }
    
    /*vector<Attachment> attachments = doc.getAllAttachments();
        for(unsigned int i=0; i<attachments.size(); i++){
            AttachedObject obj;
        
            obj.name = QString(attachments[i].getID().c_str());
            obj.type = ATTACHMENT;
            string url = "http://localhost:5984/addresses/";
            url.append(doc.getID());
            url.append("/");
            url.append(attachments[i].getID());
            obj.url = QUrl(QString(url.c_str()));
            
            files.append(obj);
            
        
        }*/
        update();
    
}






