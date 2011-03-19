
#include <QtGui>
#include <QPen>
#include <QPropertyAnimation>
#include <QDebug>
#include <iostream>

#include "textedit.h"
#include "lineedit.h"
#include "view.h"
#include "dateedit.h"
#include "combo.h"
#include "hotspot.h"
#include "action.h"
#include "attachments.h"


using namespace std;


View::View( QCouch& couch, QWidget* parent) : QWidget(parent), couch(couch)
 {
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
     
     /*editMenu = menuBar()->addMenu("&Edit");
     
     editMenu->addAction(beginEditingAction);
	 editMenu->addAction(addFieldAction);
     */
     
     setAcceptDrops(true);
     
     attachments = new Attachments(this);
     attachments->setGeometry(0,height(),width(),100);
     attachments->show();
     
     isEditing = false;
     dropActive = false;
     attachmentsVisible = false;
     
     connect(attachments, SIGNAL(fileAttached(QUrl)), this, SLOT(fileAttached(QUrl)));
     
     QPushButton *newButton = new QPushButton(tr("&New"));
     QPushButton *saveButton = new QPushButton(tr("&Save"));
     QPushButton *deleteButton = new QPushButton(tr("&Delete"));
     
     QToolButton *editButton = new QToolButton(this);
     editButton->setIcon(QIcon("icons/edit.png"));

     connect(saveButton, SIGNAL(clicked()), this, SLOT(saveDocument()));
     connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteDocument()));
     connect(newButton, SIGNAL(clicked()), this, SLOT(newDocument()));
     
     connect(editButton, SIGNAL(clicked()), this, SLOT(beginEditing()));
     
     QVBoxLayout *buttonLayout1 = new QVBoxLayout;
     buttonLayout1->addWidget(editButton, Qt::AlignTop);
     buttonLayout1->addSpacing(50);
     buttonLayout1->addWidget(newButton, Qt::AlignTop);
     buttonLayout1->addWidget(saveButton);
     buttonLayout1->addWidget(deleteButton);
     buttonLayout1->addStretch();
     
     
     QGridLayout *mainLayout = new QGridLayout;
     
     documentLayout = new QGridLayout;
     mainLayout->addLayout(documentLayout, 0, 0);
     mainLayout->addLayout(buttonLayout1, 0, 1, Qt::AlignRight);
     setLayout(mainLayout);
     
 }

void View::setDatabase(QString _database){
     database = _database;
 }

void View::setDesign(QString _design){
   design = _design;    
}

void View::setView(QString _view){
   view = _view;   
}

void View::saveDocument(){
    foreach(TemplateWidget* widget, widgets){
        widget->saveChanges(currentDoc);
    }
    try {
        QString newRevision = couch.updateDocument(currentDoc.getSourceDatabase(), currentDoc.getId(), currentDoc.getRevision(), QVariant(currentDoc.getMap()));
        currentDoc.setRevision(newRevision);
        emit documentUpdated(currentDoc);
    } catch (CouchException& e){
        QString str = "Unable to update document -- ";
        str.append(e.what());
        QMessageBox::critical(this, "Hot Tub", str);
    }

}
 

void View::newDocument(){
    Document doc;
    doc.setId(couch.getUUID());
    doc.setSourceDatabase(database);
    loadDocument(doc);
    emit(documentAdded(doc));
}


void View::clear(){
   removeAllWidgets();
}


void View::reset(){
    foreach(TemplateWidget* widget, widgets){
        widget->reset();
    }
}

void View::deleteDocument(){
    QMessageBox msgBox;
    msgBox.setText("You are about to delete this document.");
    msgBox.setInformativeText("Are you sure you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Yes ) {
        couch.deleteDocument(currentDoc.getSourceDatabase(), currentDoc.getId(), currentDoc.getRevision());
        reset();
        emit(documentDeleted(currentDoc));
    }
}


void View::saveTemplate(){
    
    QList<QVariant> fields = QList<QVariant>();
	
    for(int i=0; i<widgets.size(); i++){
        QVariantMap fieldMap = QVariantMap();
        TemplateWidget* widget = widgets[i];
        
        fieldMap["key"] = widget->getField();
        fieldMap["x"] = widget->x();
        fieldMap["y"] = widget->y();
        fieldMap["width"] = widget->width();
        fieldMap["height"] = widget->height();
        fieldMap["editor"] = widget->getEditorType(); 
        
        fields.append(QVariant(fieldMap));   
    }
    
    
    QVariantMap _template = QVariantMap();
    _template["fields"] = QVariant(fields);
    _template["design"] = design;
    _template["view"] = view;
    _template["type"] = "template";
    
    QVariant var = QVariant(_template);
	
    try {
        Document doc = findTemplate();
        couch.updateDocument(database, doc.getId(), doc.getRevision(), var);
    }catch(int){
        QString id = couch.getUUID(); 
    	couch.createDocument(database, id, var);
    }
}

Document View::findTemplate(){
    
    /* Check for an existing template */
    QList<QVariant> startKey = QList<QVariant>();
    startKey.append(design);
    startKey.append(view);
    
    QList<QVariant> endKey = QList<QVariant>();
    endKey.append(design);
    endKey.append(view);
    endKey.append(QVariantMap());
    
    QList<QVariant> results = couch.getView(database, "_design/templates", "all", QVariant(startKey), QVariant(endKey));
    if ( results.size() == 1 ) {
        QVariantMap map = results[0].toMap();
        Document templateDoc = couch.getDocument(database, map["id"].toString());
        return templateDoc;
    }
    throw 2;
}

void View::loadDocument(Document doc){
    int x = 50;
    int y = 50;
    currentDoc = doc;
    removeAllWidgets();

    try {
        Document templateDoc = findTemplate();
        QVariantMap templateMap = templateDoc.getMap();
        QList<QVariant> fields = templateMap["fields"].toList();
        foreach(QVariant field, fields){
            QVariantMap fieldMap = field.toMap();
            QString editorType = fieldMap["editor"].toString();
            QString key = fieldMap["key"].toString();
            int x = fieldMap["x"].toInt();
            int y = fieldMap["y"].toInt();
            int width = fieldMap["width"].toInt();
            int height = fieldMap["height"].toInt();
            Editor *editor;
            
            if( editorType == "lineedit" ) {
                editor = new LineEdit(this);
            } else if ( editorType == "textedit" ) {
                editor = new TextEdit(this);
            } else if ( editorType == "combo" ) {
                editor = new Combo(this);
            } else if ( editorType == "dateedit" ) {
                editor = new DateEdit(this);
            }
                
            TemplateWidget *widget = new TemplateWidget(editor, this);
            widget->setLabel(key);
            widget->setField(key);
            widget->loadDocument(doc);
    		QSize hint = widget->sizeHint();
            if ( width == 0 ) width = hint.width();
            if ( height == 0 ) height = hint.height();
                
            widget->setGeometry(x,y,width,height);
            widget->show();
            widgets.push_back(widget);
        
        }
    } catch (int) {
        /* None found, generate one */
    
    	TemplateWidget *widget = new TemplateWidget(new LineEdit(this), this);
    	widget->setLabel("_id");
    	widget->setField("_id");
    	widget->loadDocument(doc);
    	
    	QSize hint = widget->sizeHint();
    	widget->setGeometry(x,y,hint.width(), hint.height());
    	widget->show();
    	widgets.push_back(widget);
    	
    	y += 30;
	
    	
    	
    	QVariantMap map = doc.getMap();
    	   foreach(QString key, map.keys() ){
        	if ( key != "_rev" && key != "_attachments" && key != "_id" ) {
          	  widget = new TemplateWidget(new LineEdit(this), this);
            	  widget->setLabel(key);
            	  widget->setField(key);
            	  widget->loadDocument(doc);
            	  QSize hint = widget->sizeHint();
            	  widget->setGeometry(x,y,hint.width(), hint.height());
            	  widget->show();
            	  widgets.push_back(widget);


	                     
                  y += 30;
            }
        }

        saveTemplate();
    }
    	
    attachments->loadDocument(doc);
}

void View::fileAttached(QUrl url) {
    
    QString canonicalFilename = url.toLocalFile();
    QString filename = canonicalFilename.split("/").last();
    QFile file(canonicalFilename);
    if ( file.open(QIODevice::ReadOnly) ) {
        couch.putAttachment(currentDoc.getSourceDatabase(), currentDoc.getId(), currentDoc.getRevision(), filename, &file);
        file.close();
    }
    
    
}

void View::removeAllWidgets(){
    for (int i=0; i<widgets.size(); i++) {
        delete widgets[i];
    }
    widgets.clear();
    
}
 
void View::dragEnterEvent(QDragEnterEvent *event)
{
    showAttachments();
    dropActive = true;
    event->acceptProposedAction();
}

void View::showAttachments(){
    if ( !attachmentsVisible ){ 
        attachmentsVisible = true;
    
    
        QPropertyAnimation* animation = new QPropertyAnimation(attachments, "geometry");
        animation->setDuration(150);
        animation->setEasingCurve(QEasingCurve::InOutQuart);
        animation->setEndValue(QRect(0,height()-100,width(),100));
        animation->start();
    }
    
}

void View::hideAttachments(){
    if ( dropActive ) return;
    
    attachmentsVisible = false;
    
    QPoint pos = mapFromGlobal(QCursor::pos());
    if ( pos.y() < height() - 100 ) {
        QPropertyAnimation* animation = new QPropertyAnimation(attachments, "geometry");
        animation->setDuration(300);
        animation->setEasingCurve(QEasingCurve::InOutQuart);
        animation->setEndValue(QRect(0,height(),width(),100));
        animation->start();
    }
    
}


void View::dragLeaveEvent(QDragLeaveEvent *event){

    Q_UNUSED(event);
    dropActive = false;
    QTimer::singleShot(1500, this, SLOT(hideAttachments()));

    
}

void View::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
    
    if ( event->mimeData()->hasUrls() ) {
        QList<QUrl> urls = event->mimeData()->urls();
        for(int i=0; i<urls.length(); i++){
            attachments->acceptUrl(urls[i]);
        }
    }
    
    dropActive = false;
    
    QTimer::singleShot(1500, this, SLOT(hideAttachments()));
    
}

void View::widgetRemoved(TemplateWidget *widget){
    
    for (int i=0; i<widgets.size(); i++) {
        if ( widgets[i] == widget ){
            widgets.erase(widgets.begin()+i);
        }
    }
    
    delete widget;
    
}

void View::changeButtonPushed(){
    /*date->hide();
    
    LineEdit *line = new LineEdit("Birthday", this);
    line->move(date->x(), date->y());
    line->show();
    line->beginEditing();
    */
}

void View::beginEditing(){
    if (isEditing) {
        isEditing = false;
        beginEditingAction->setText("Start Editing");
        for (int i=0; i<widgets.size(); i++) {
            widgets[i]->stopEditing();
        }
        
        saveTemplate();
        
    } else {
        isEditing = true;
        beginEditingAction->setText("Stop Editing");
        
        for (int i=0; i<widgets.size(); i++) {
            widgets[i]->beginEditing();
            /*QPushButton *button = new QPushButton("Change", this);
            button->move(widgets[i]->x()+widgets[i]->width()-20, widgets[i]->y()+widgets[i]->height()-20);
            button->show();
            connect(button, SIGNAL(clicked()), this, SLOT(changeButtonPushed()));
        */}
        
    }
}

void View::addField(){
    TemplateWidget *widget = new TemplateWidget(new LineEdit(this), this);
    widget->setLabel("New Field");
    
    QSize hint = widget->sizeHint();
    widget->setGeometry(0,0,hint.width(), hint.height());
    widget->show();
    widget->beginEditing();
    widgets.push_back(widget);
    
    connect(widget, SIGNAL(remove(TemplateWidget *)), this, SLOT(widgetRemoved(TemplateWidget *)));
    
}

void View::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    
    attachments->setGeometry(0,height(),width(),100);
}



void View::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);
    
     
     if ( !overrideHints ) { 
        if ( hintX != -1 || hintY != -1 ) {
            QPen pen;
            pen.setStyle(Qt::DotLine);
            pen.setColor(Qt::gray);
     
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

void View::buildHotSpots(){
    hotSpots.clear();
    int x,y,width,height;
    for( int i=0; i<widgets.size(); i++){
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

 void View::mouseReleaseEvent(QMouseEvent *event)
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

 void View::mousePressEvent(QMouseEvent *event)
 {
     if ( !isEditing ) return;
     
     activeAction = true;
     QWidget *child = NULL;
	 
     for (int i=0; i<widgets.size(); i++) {
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

/*void View::activeCellChanged(){

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

void View::mouseLeaveEvent( QMouseEvent *event ){
    Q_UNUSED(event);
    QTimer::singleShot(1500, this, SLOT(hideAttachments()));
}

void View::mouseMoveEvent( QMouseEvent *event ) {
	
    if ( event->pos().y() > height()-100 ) {
        showAttachments();
    } else {
        QTimer::singleShot(1500, this, SLOT(hideAttachments()));
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
    for(int i=0; i<widgets.size(); i++){
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


void View::keyPressEvent( QKeyEvent *event ) {
    if ( event->modifiers() && Qt::ControlModifier ) {
        overrideHints = true;
        update();
    }
    
}


void View::keyReleaseEvent( QKeyEvent *event ) {
    Q_UNUSED(event);
    
    overrideHints = false;
    update();
}







