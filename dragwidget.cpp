
#include <QtGui>
#include <QPen>
#include <QPropertyAnimation>
#include <iostream>

#include "textedit.h"
#include "lineedit.h"
#include "dragwidget.h"
#include "dateedit.h"

using namespace std;


 DragWidget::DragWidget(QWidget *parent)
     : QWidget(parent)
 {
     TextEdit *edit = new TextEdit("Address", this);
     LineEdit *line = new LineEdit("Name", this);
     DateEdit *date = new DateEdit("Birthday", this);
     
     line->move(0,0);
     line->show();
     edit->move(0,50);
     edit->show();
     
     date->move(0,100);
     date->show();
     
     date->enterEditMode();
     line->enterEditMode();
     edit->enterEditMode();
     
     widgets.push_back(date);
     widgets.push_back(line);
     widgets.push_back(edit);
     
     QPalette newPalette = palette();
     newPalette.setColor(QPalette::Window, Qt::white);
     setPalette(newPalette);

     setMinimumSize(800, 600);
     
	 activeDragging = false;

     hintX = -1;
     hintY = -1;
     overrideHints = false;
	 timer = new QTimer(this);
	 //connect(timer, SIGNAL(timeout()), this, SLOT(activeCellChanged()));
	 
 }


 void DragWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

     if ( overrideHints ) return; 
     
     if ( hintX != -1 || hintY != -1 ) {
         QPen pen;
         pen.setStyle(Qt::DotLine);
         pen.setColor(Qt::gray);
         QPainter painter(this);
     
         painter.setPen(pen);
         
         if ( hintX != -1 ) {
             painter.drawLine(hintX, 0, hintX, height());
         }
     
         if ( hintY != -1 ) {
             painter.drawLine(0, hintY, width(), hintY);
         }
     }
 }

void showMessage(QString message){
	QMessageBox msgBox;
	msgBox.setText(message);
	msgBox.exec();
}

 void DragWidget::mouseReleaseEvent(QMouseEvent *event)
 {
     Q_UNUSED(event);
     
     if ( activeWidget != NULL ) {
     	activeWidget->setCursor(Qt::OpenHandCursor);
	 }
        
     activeDragging = false;
	 activeWidget = NULL;
     
     hintX = -1;
     hintY = -1;
     update();
 }

 void DragWidget::mousePressEvent(QMouseEvent *event)
 {
     QWidget *child = childAt(event->pos());
	 if ( child == NULL ) return;

	 QWidget *parent = child->parentWidget();
	 while ( parent != this ) {
		child = parent;
		parent = parent->parentWidget();
	 }

	 TemplateWidget *templWidget = static_cast<TemplateWidget*>(child);
	 if ( templWidget ) {
	 	templWidget->setCursor(Qt::ClosedHandCursor);
	 }


	 offsetX = event->pos().x() - child->x();
	 offsetY = event->pos().y() - child->y();

	 activeWidget = templWidget;
	 activeWidget->raise();
	 activeDragging = true;
 }

/*void DragWidget::activeCellChanged(){

	if ( activeWidget == NULL ) return;

	int row = activeRow;
	int m = activeWidget->height() / cellHeight + 1;
	cout << activeWidget->height() << " " << cellHeight << " " << m << endl;
	int dir = 1;
	if ( lastActiveRow < activeRow ) {
		dir = -1;
	}
 	QAnimationGroup* animationGroup = new QParallelAnimationGroup();
	while ( grid[row][activeCol] != NULL && ( row != originRow || activeCol != originCol) ){ 
		TemplateWidget* widget = grid[row][activeCol];
		QPropertyAnimation* animation = new QPropertyAnimation(widget, "geometry");
  		animation->setDuration(150);
 		animation->setEasingCurve(QEasingCurve::InOutQuart);
 		animation->setEndValue(QRect(widget->x(),widget->y()+(cellHeight*dir*m),widget->width(),widget->height()));

		animationGroup->addAnimation(animation);

		row = row + dir;
	}	


	if ( row != activeRow ){ 
 		animationGroup->start(QAbstractAnimation::DeleteWhenStopped);		
	
		while ( row != activeRow ) {
			grid[row][activeCol] = grid[row-(dir*m)][activeCol];
			row = row - dir;
		}
		grid[row][activeCol]=NULL;
	}

}*/

void DragWidget::mouseMoveEvent( QMouseEvent *event ) {
	if ( activeDragging ) {
		activeWidget->move(event->pos().x()-offsetX, event->pos().y()-offsetY);
	}
    
    if ( overrideHints ) return;
    
    hintX = -1;
    hintY = -1;
    int activeWidgetHintX = activeWidget->getLeftAlignmentHint();
    int activeWidgetHintY = activeWidget->getTopAlignmentHint();
    for(unsigned int i=0; i<widgets.size(); i++){
        if ( widgets[i] != activeWidget ) {
            int hint = widgets[i]->getLeftAlignmentHint() + widgets[i]->x();
        
            if ( abs(hint - (event->pos().x() + activeWidgetHintX) + offsetX ) < THRESHOLD ) {
                activeWidget->move(hint - activeWidgetHintX, activeWidget->y());
                hintX = hint;
            }
            
            hint = widgets[i]->getTopAlignmentHint() + widgets[i]->y();
            if ( abs( hint - (event->pos().y() + activeWidgetHintY) + offsetY ) < THRESHOLD ) {
                activeWidget->move(activeWidget->x(), hint - activeWidgetHintY);
                hintY = hint;
            }
        }
    }
    update();

}


void DragWidget::keyPressEvent( QKeyEvent *event ) {
    if ( event->modifiers() && Qt::ControlModifier ) {
        overrideHints = true;
        update();
    }
    
}


void DragWidget::keyReleaseEvent( QKeyEvent *event ) {
    Q_UNUSED(event);
    
    overrideHints = false;
    update();
}



