
#include <QtGui>
#include <QPen>
#include <QPropertyAnimation>
#include <iostream>

#include "textedit.h"
#include "lineedit.h"
#include "dragwidget.h"
#include "dateedit.h"
#include "hotspot.h"
#include "action.h"
#include "attachments.h"

using namespace std;


DragWidget::DragWidget(QWidget* parent) : QMainWindow(parent)
 {
     setMinimumSize(800, 600);
     
	 activeDragging = false;
     activeWidget = NULL;
     
     actionWidget = NULL;
     activeAction = false;
     

     hintX = -1;
     hintY = -1;
     overrideHints = false;
	 timer = new QTimer(this);
     
     setMouseTracking(true);
     
     beginEditingAction = new QAction("Start Editing", this);
     connect(beginEditingAction, SIGNAL(triggered()), this, SLOT(beginEditing()));
     
     addFieldAction = new QAction("Add A Field", this);
     connect(addFieldAction, SIGNAL(triggered()), this, SLOT(addField()));
     
     editMenu = menuBar()->addMenu("&Edit");
     
     editMenu->addAction(beginEditingAction);
	 editMenu->addAction(addFieldAction);
     
     setAcceptDrops(true);
     
     attachments = new Attachments(this);
     attachments->setGeometry(0,height(),width(),100);
     attachments->show();
     
     isEditing = false;
     attachmentsVisible = false;
     
 }

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    showAttachments();
    
    event->acceptProposedAction();
}

void DragWidget::showAttachments(){
    if ( !attachmentsVisible ){ 
        attachmentsVisible = true;
    
    
        QPropertyAnimation* animation = new QPropertyAnimation(attachments, "geometry");
        animation->setDuration(150);
        animation->setEasingCurve(QEasingCurve::InOutQuart);
        animation->setEndValue(QRect(0,height()-100,width(),100));
        animation->start();
    }
    
}

void DragWidget::hideAttachments(){
    attachmentsVisible = false;
    
    QPoint pos = mapFromGlobal(QCursor::pos());
    if ( pos.y() < height() - 100 ) {
        QPropertyAnimation* animation = new QPropertyAnimation(attachments, "geometry");
        animation->setDuration(150);
        animation->setEasingCurve(QEasingCurve::InOutQuart);
        animation->setEndValue(QRect(0,height(),width(),100));
        animation->start();
    }
    
}


void DragWidget::dragLeaveEvent(QDragLeaveEvent *event){

    Q_UNUSED(event);
    
    hideAttachments();
    
}

void DragWidget::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
    
    if ( event->mimeData()->hasUrls() ) {
        QList<QUrl> urls = event->mimeData()->urls();
        for(int i=0; i<urls.length(); i++){
            attachments->acceptUrl(urls[i]);
        }
    }
    
    QTimer::singleShot(750, this, SLOT(hideAttachments()));
    
}

void DragWidget::widgetRemoved(TemplateWidget *widget){
    
    for (unsigned int i=0; i<widgets.size(); i++) {
        if ( widgets[i] == widget ){
            widgets.erase(widgets.begin()+i);
        }
    }
    
    delete widget;
    
}

void DragWidget::changeButtonPushed(){
    /*date->hide();
    
    LineEdit *line = new LineEdit("Birthday", this);
    line->move(date->x(), date->y());
    line->show();
    line->beginEditing();
    */
}

void DragWidget::beginEditing(){
    if (isEditing) {
        isEditing = false;
        beginEditingAction->setText("Start Editing");
        for (unsigned int i=0; i<widgets.size(); i++) {
            widgets[i]->stopEditing();
        }
    } else {
        isEditing = true;
        beginEditingAction->setText("Stop Editing");
        
        for (unsigned int i=0; i<widgets.size(); i++) {
            widgets[i]->beginEditing();
            /*QPushButton *button = new QPushButton("Change", this);
            button->move(widgets[i]->x()+widgets[i]->width()-20, widgets[i]->y()+widgets[i]->height()-20);
            button->show();
            connect(button, SIGNAL(clicked()), this, SLOT(changeButtonPushed()));
        */}
    }
}

void DragWidget::addField(){
    TemplateWidget *widget = new TemplateWidget(new LineEdit(this), this);
    widget->setLabel("New Field");
    
    QSize hint = widget->sizeHint();
    widget->setGeometry(0,0,hint.width(), hint.height());
    widget->show();
    widget->beginEditing();
    widgets.push_back(widget);
    
    connect(widget, SIGNAL(remove(TemplateWidget *)), this, SLOT(widgetRemoved(TemplateWidget *)));
    
}

void DragWidget::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    
    attachments->setGeometry(0,height(),width(),100);
}



 void DragWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

     
     
     if ( !overrideHints ) { 
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
     
     /*QPainter painter(this);
     for (unsigned int i=0; i<hotSpots.size(); i++) {
         painter.fillRect(hotSpots[i].rect(), Qt::yellow);
     }*/
 }


void showMessage(QString message){
	QMessageBox msgBox;
	msgBox.setText(message);
	msgBox.exec();
}

void DragWidget::buildHotSpots(){
    hotSpots.clear();
    int x,y,width,height;
    for( unsigned int i=0; i<widgets.size(); i++){
        if ( widgets[i]->allowResizeWidth() ) {
            x=widgets[i]->x();
            y=widgets[i]->y();
            width=widgets[i]->width();
            height=widgets[i]->height();
      
            if ( widgets[i]->allowResizeWidth() && widgets[i]->allowResizeHeight() ) {
                hotSpots.push_back(Hotspot(widgets[i], QRect(x+width-THRESHOLD/2, y, THRESHOLD, height-THRESHOLD/2), Qt::SizeHorCursor, RESIZE_WIDTH));
                hotSpots.push_back(Hotspot(widgets[i], QRect(x,y+height-THRESHOLD/2, width-THRESHOLD/2, THRESHOLD), Qt::SizeVerCursor, RESIZE_HEIGHT));
                hotSpots.push_back(Hotspot(widgets[i], QRect(x+width-THRESHOLD/2, y+height-THRESHOLD/2, THRESHOLD, THRESHOLD), Qt::SizeFDiagCursor, RESIZE_BOTH));
            } else if ( widgets[i]->allowResizeHeight() ) {
                hotSpots.push_back(Hotspot(widgets[i], QRect(x,y+height-THRESHOLD/2, width, THRESHOLD), Qt::SizeVerCursor, RESIZE_HEIGHT));
            } else if ( widgets[i]->allowResizeWidth() ) {
                hotSpots.push_back(Hotspot(widgets[i], QRect(x+width-THRESHOLD/2, y, THRESHOLD, height), Qt::SizeHorCursor, RESIZE_WIDTH));
            }
        }
    }
}

 void DragWidget::mouseReleaseEvent(QMouseEvent *event)
 {
     Q_UNUSED(event);
     if ( !isEditing ) return;
     
     buildHotSpots();
     
     if ( activeWidget ) {
         activeWidget->setCursor(Qt::OpenHandCursor);
     }
     
     activeDragging = false;
	 activeWidget = NULL;
     
     activeAction = false;
     action = NONE;
     
     hintX = -1;
     hintY = -1;
     marginY = -1;
     update();
 }

 void DragWidget::mousePressEvent(QMouseEvent *event)
 {
     if ( !isEditing ) return;
     
     activeAction = true;
     QWidget *child = NULL;
	 
     for (unsigned int i=0; i<widgets.size(); i++) {
         if ( widgets[i]->geometry().contains(event->pos())) {
             child = widgets[i];
             continue;
         }
     }

     if ( child == NULL ) return;

	 /*QWidget *parent = child->parentWidget();
	 while ( parent != this ) {
		child = parent;
		parent = parent->parentWidget();
	 }*/

	 TemplateWidget *templWidget = static_cast<TemplateWidget*>(child);
	 
	 offsetX = event->pos().x() - child->x();
	 offsetY = event->pos().y() - child->y();

	 activeWidget = templWidget;
     activeWidget->setCursor(Qt::ClosedHandCursor);
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

void DragWidget::mouseLeaveEvent( QMouseEvent *event ){
    Q_UNUSED(event);
    hideAttachments();
}

void DragWidget::mouseMoveEvent( QMouseEvent *event ) {
	
    if ( event->pos().y() > height()-100 ) {
        showAttachments();
    } else {
        hideAttachments();
    }
    
    
    if ( !isEditing ) return;
    
    if ( activeAction && action == RESIZE_BOTH ) {
        int width = qMax(actionWidget->minimumWidth(), event->pos().x() - actionWidget->x());
        int height = qMax(actionWidget->minimumHeight(), event->pos().y() - actionWidget->y());
        actionWidget->setGeometry(actionWidget->x(), actionWidget->y(), width, height );
        return;
    } else if ( activeAction && action == RESIZE_WIDTH ) {
        int width = qMax(actionWidget->minimumWidth(), event->pos().x() - actionWidget->x());
        actionWidget->setGeometry(actionWidget->x(), actionWidget->y(), width, actionWidget->height() );
        return;
    } else if ( activeAction && action == RESIZE_HEIGHT ) {
        int height = qMax(actionWidget->minimumHeight(), event->pos().y() - actionWidget->y());
        actionWidget->setGeometry(actionWidget->x(), actionWidget->y(), actionWidget->width(), height );
        return;
    }
        
    
    if ( activeDragging ) {
        activeWidget->move(event->pos().x()-offsetX, event->pos().y()-offsetY);
        
    }
        
    
    setCursor(Qt::ArrowCursor);
    action = NONE;
    actionWidget = NULL;
    
    hintX = -1;
    hintY = -1;
    marginY = -1;
    int activeWidgetHintX=-1, activeWidgetHintY=-1;
    
    if ( activeWidget != NULL ){
        activeWidgetHintX = activeWidget->getLeftAlignmentHint();
        activeWidgetHintY = activeWidget->getTopAlignmentHint();
    }
    for(unsigned int i=0; i<widgets.size(); i++){
        if ( widgets[i] != activeWidget ) {
            if ( activeWidget != NULL && !overrideHints ) {
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
            
            /* Check for margins */
                if ( hintX > -1 ) {
                    if ( abs(event->pos().y() - widgets[i]->y() - widgets[i]->height() - offsetY) < THRESHOLD ) {
                        activeWidget->move(activeWidget->x(), widgets[i]->y() + widgets[i]->height());
                    }
           
                    if ( abs(event->pos().y() - offsetY - widgets[i]->y() + activeWidget->height() ) < THRESHOLD ) {
                        activeWidget->move(activeWidget->x(), widgets[i]->y() - activeWidget->height());
                    }
                }
            }
        }
        
        if ( !activeDragging ) {
            /* Check hot spots */
            for (unsigned int i=0; i<hotSpots.size(); i++) {
                if ( hotSpots[i].rect().contains(event->pos() ) ) {
                    setCursor(hotSpots[i].getCursor());
                    action = hotSpots[i].getAction();
                    actionWidget = hotSpots[i].targetWidget();
                }
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







