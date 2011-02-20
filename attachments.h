

#ifndef __ATTACHMENTS_H
#define __ATTACHMENTS_H

#include <QtGui>
#include "couchdb/CouchDB.hpp"

using namespace CouchDB;

enum AttachmentType {
    ATTACHMENT,
    FOLDER
};

typedef struct {
    QString name;
    AttachmentType type;
    QTemporaryFile* temporaryFile;
} AttachedObject;

class Attachments : public QWidget {
    Q_OBJECT
    
public:
    Attachments(QWidget *parent);
    QSize sizeHint();
    void paintEvent(QPaintEvent *event);
    void acceptUrl(QUrl url);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    
    void loadDocument(Document &);
signals:
    void fileAttached(QUrl url);
    
private slots:
    void addFolder();
    void openObject();
    void deleteObject();
    
private:
    
    bool hasSelectedObject();
    
    QList<AttachedObject> files;
  
    static const int iconWidth = 44;
    static const int iconHeight = 57;
    
    int columnWidth;
    int columnHeight;
    int rowHeight;
    int columns;
    int leftmargin;
    int selectedColumn;
    
    QMenu *contextMenu;
    QPoint dragStartPosition;
    
    QAction *openAction;
    QAction *deleteAction;
    QAction *newFolderAction;

};



#endif
