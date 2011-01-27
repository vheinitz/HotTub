

#ifndef __ATTACHMENTS_H
#define __ATTACHMENTS_H

#include <QtGui>


enum AttachmentType {
    ATTACHMENT,
    FOLDER
};

typedef struct {
    QString name;
    AttachmentType type;
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
    void contextMenuEvent(QContextMenuEvent *event);
    
private slots:
    void addFolder();
    
private:
    
    QList<AttachedObject> files;
  
    static const int iconWidth = 44;
    static const int iconHeight = 57;
    
    int columnWidth;
    int rowHeight;
    int columns;
    int leftmargin;
    int selectedColumn;
    
    QMenu *contextMenu;
};



#endif
