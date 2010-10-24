

 #ifndef DRAGWIDGET_H
 #define DRAGWIDGET_H

 #include <QWidget>
 #include <QtGui>
#include <vector>
 #include "templatewidget.h"

 class QDragEnterEvent;
 class QDropEvent;

using namespace std;


 class DragWidget : public QWidget
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

 private:
     vector<int> leftAlignmentHints;
	 vector<TemplateWidget*> widgets;
     TemplateWidget* activeWidget;
	 QLabel *label;
	 bool activeDragging;
	 
	 int offsetX;
	 int offsetY;
   
     int hintX;
     int hintY;
     
     bool overrideHints;
     
     static const int THRESHOLD = 25;

	 QTimer *timer;
 };

 #endif
