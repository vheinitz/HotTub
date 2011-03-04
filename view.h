

#ifndef __VIEW_H
#define __VIEW_H

#include <QWidget>
#include <QtGui>
#include <vector>
#include "templatewidget.h"
#include "hotspot.h"
#include "action.h"
#include "dateedit.h"
#include "attachments.h"
#include "qcouch/document.h"

 class QDragEnterEvent;
 class QDropEvent;

using namespace std;

 class View : public QWidget
 {
	Q_OBJECT
			
 public:
     View( QWidget *parent = 0);
     
 protected:
	 void mouseMoveEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mouseLeaveEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent *event);
     void keyReleaseEvent(QKeyEvent *event);
	 void paintEvent(QPaintEvent *event);
     void dragEnterEvent(QDragEnterEvent *event);
     void dragLeaveEvent(QDragLeaveEvent *event);
     void dropEvent(QDropEvent *event);
     void resizeEvent(QResizeEvent *event);
     
     void removeAllWidgets();
   
private slots:
     void beginEditing();
     void addField();
     void changeButtonPushed();
     void widgetRemoved(TemplateWidget *);
     void hideAttachments();
     void showAttachments();
     void fileAttached(QUrl url);
     
 private:
     void buildHotSpots();
     
     vector<int> leftAlignmentHints;
     vector<Hotspot> hotSpots;
	 vector<TemplateWidget*> widgets;
     
     TemplateWidget* activeWidget;
     QWidget* actionWidget;
     
     QLabel *label;
	 bool activeDragging;
	 bool dropActive;
     
	 int offsetX;
	 int offsetY;
     
     TemplateWidget *date;
   
     bool attachmentsVisible;
     
     int hintX;
     int hintY;
     
     int marginY;
     
     bool overrideHints;
     
     static const int THRESHOLD = 25;
     static const int MARGIN = 1;

	 QTimer *timer;
     
     Action action;
     bool activeAction;
     
     bool isEditing;
     
     QMenu *editMenu;
     QAction *beginEditingAction;
     QAction *addFieldAction;
     
     Attachments *attachments;

 };


 #endif
