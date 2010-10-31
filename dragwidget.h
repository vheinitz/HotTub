

#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QtGui>
#include <vector>
#include "templatewidget.h"
#include "hotspot.h"
#include "action.h"

 class QDragEnterEvent;
 class QDropEvent;

using namespace std;


 class DragWidget : public QMainWindow
 {
	Q_OBJECT
			
 public:
     DragWidget(QWidget *parent = 0);

 protected:
	 void mouseMoveEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent *event);
     void keyReleaseEvent(QKeyEvent *event);
	 void paintEvent(QPaintEvent *event);

private slots:
     void beginEditing();
     
 private:
     void buildHotSpots();
     
     vector<int> leftAlignmentHints;
     vector<Hotspot> hotSpots;
	 vector<TemplateWidget*> widgets;
     
     TemplateWidget* activeWidget;
     QWidget* actionWidget;
     
     QLabel *label;
	 bool activeDragging;
	 
	 int offsetX;
	 int offsetY;
   
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
     
 };


 #endif
