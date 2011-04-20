
#include <QtGui>
#include <QPen>
#include <QPropertyAnimation>
#include <QDebug>
#include <iostream>

#include "textedit.h"
#include "lineedit.h"
#include "listedit.h"
#include "view.h"
#include "dateedit.h"
#include "combo.h"
#include "grid.h"
#include "hotspot.h"
#include "action.h"
#include "attachments.h"


using namespace std;



bool widgetLocationSort(TemplateWidget* a, TemplateWidget* b){
    return a->y() < b->y();
}

View::View( QCouch& couch, QWidget* parent) : QWidget(parent), couch(couch)
 {
     activeDragging = false;
     activeWidget = NULL;

	 selectedWidget = NULL;
     
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
     
     attachments = new Attachments(couch, this);
     attachments->setGeometry(0,height(),width(),100);
     attachments->show();
     
     isEditing = false;
     dropActive = false;
     attachmentsVisible = false;
     
     connect(attachments, SIGNAL(fileAttached(QUrl)), this, SLOT(fileAttached(QUrl)));
     
     newButton = new QPushButton(tr("&New"));
     saveButton = new QPushButton(tr("&Save"));
     deleteButton = new QPushButton(tr("&Delete"));
     
     editButton = new QPushButton(tr("Edit Template"));
     addFieldButton = new QPushButton(tr("&Add Field"));
     
     addFieldButton->setVisible(false);

     connect(saveButton, SIGNAL(clicked()), this, SLOT(saveDocument()));
     connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteDocument()));
     connect(newButton, SIGNAL(clicked()), this, SLOT(newDocument()));
     
     
     QVBoxLayout *buttonLayout1 = new QVBoxLayout;
     buttonLayout1->addSpacing(50);
     buttonLayout1->addWidget(newButton, Qt::AlignTop);
     buttonLayout1->addWidget(saveButton);
     buttonLayout1->addWidget(deleteButton);
     
     buttonLayout1->addSpacing(50);
     buttonLayout1->addWidget(editButton, Qt::AlignTop);
     buttonLayout1->addWidget(addFieldButton);
     
     connect(editButton, SIGNAL(clicked()), this, SLOT(beginEditing()));
     connect(addFieldButton, SIGNAL(clicked()), this, SLOT(addField()));
     
     buttonLayout1->addStretch();
     
     QGridLayout *mainLayout = new QGridLayout;
     
     documentLayout = new QGridLayout;
     mainLayout->addLayout(documentLayout, 0, 0);
     mainLayout->addLayout(buttonLayout1, 0, 1, Qt::AlignRight);
     setLayout(mainLayout);

	toolbar = new QToolBar(this);
    toolbar->setFloatable(true);
    
    QAction *changeAction = new QAction(toolbar);
    changeAction->setIcon(QIcon("icons/preferences-system.png"));
    
    connect(changeAction, SIGNAL(triggered()), this, SLOT(showChangeEditorMenu()));
    
    QAction *deleteAction = new QAction(toolbar);
    deleteAction->setIcon(QIcon("icons/user-trash.png"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteWidget())); 
    
    toolbar->addAction(changeAction);
    
    toolbar->addAction(deleteAction);
    toolbar->setVisible(false);

    changeEditorMenu = new QMenu(toolbar);
    
    QAction *textAction = new QAction(changeEditorMenu);
    textAction->setText("Text");
    
    QAction *dateAction = new QAction(changeEditorMenu);
    dateAction->setText("Date");
    
    QAction *areaAction = new QAction(changeEditorMenu);
    areaAction->setText("Text Area");
    
    QAction *comboAction = new QAction(changeEditorMenu);
    comboAction->setText("Combo Box");
    
    QAction *listAction = new QAction(changeEditorMenu);
    listAction->setText("List");
    
    QAction *gridAction = new QAction(changeEditorMenu);
    gridAction->setText("Grid");
    
    changeEditorMenu->addAction(textAction);
    changeEditorMenu->addAction(dateAction);
    changeEditorMenu->addAction(areaAction);
    changeEditorMenu->addAction(comboAction);
    changeEditorMenu->addAction(listAction);
    changeEditorMenu->addAction(gridAction);

    connect(textAction, SIGNAL(triggered()), this, SLOT(changeEditorText()));
    connect(dateAction, SIGNAL(triggered()), this, SLOT(changeEditorDate()));
    connect(areaAction, SIGNAL(triggered()), this, SLOT(changeEditorTextarea()));
    connect(comboAction, SIGNAL(triggered()), this, SLOT(changeEditorComboBox()));
    connect(listAction, SIGNAL(triggered()), this, SLOT(changeEditorList()));
    connect(gridAction, SIGNAL(triggered()), this, SLOT(changeEditorGrid()));
     
    hasDocument = false;
     
 }

void View::setDatabase(QString _database){
     database = _database;
 }


void View::saveDocument(){
    foreach(TemplateWidget* widget, widgets){
        widget->saveChanges(currentDoc);
    }
    try {
        QString newRevision = couch.updateDocument(currentDoc.getSourceDatabase(), currentDoc.getId(), currentDoc.getRevision(), QVariant(currentDoc.getMap()));
        currentDoc.setRevision(newRevision);
        loadDocument(currentDoc, true);
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
    hasDocument = false;
}

bool View::hasChanges(){
    if ( hasDocument ) {
        foreach(TemplateWidget* widget, widgets){
            if ( widget->hasChanges() ) {
                qDebug() << widget->getField();
                return true;
            }
        }
    }
    return false;
}

void View::saveChanges(){
    saveDocument();
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

void View::loadTemplate(QString _design, QString _view){
    hasDocument = false;
    design = _design;
    view = _view;
    
    int x=50;
    int y=50;
    
    QList<QPair<int, TemplateWidget *> > tabOrder;
    
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
            } else if ( editorType == "listedit" ) {
                editor = new ListEdit(this);
            } else if ( editorType == "grid" ) {
                editor = new Grid(this);
            }
            
            TemplateWidget *widget = new TemplateWidget(editor, this);
            widget->setLabel(key);
            widget->setField(key);
    		QSize hint = widget->sizeHint();
            if ( width == 0 ) width = hint.width();
            if ( height == 0 ) height = hint.height();
            
            widget->setGeometry(x,y,width,height);
            widget->loadConfiguration(field);
            widget->show();
            widgets.push_back(widget);
            
            tabOrder.append(qMakePair(y, widget));
        }
    } catch (DocumentNotFoundException) {
        /* None found, generate one */
        
    	TemplateWidget *widget = new TemplateWidget(new LineEdit(this), this);
    	widget->setLabel("_id");
    	widget->setField("_id");
    	
    	QSize hint = widget->sizeHint();
    	widget->setGeometry(x,y,hint.width(), hint.height());
    	widget->show();
    	widgets.push_back(widget);
    	
    	y += 30;
        
        /* Get a sample document to decide what the generic template should look like */
        QList<QVariant> docs = couch.getView(database, design, view, QVariant(), QVariant(), false, 1 );
        if ( docs.length() > 0 ) {
            
            QVariant var = docs[0];
            QVariantMap map = var.toMap();
            if ( map.find("id") != map.end() ) {
                Document doc = couch.getDocument(database, map["id"].toString());
        
                QVariantMap map = doc.getMap();
                foreach(QString key, map.keys() ){
                    if ( key != "_attachments" && key != "_id" ) {
                        QVariant var = map[key];
                        Editor *editor;
                        if ( var.canConvert(QVariant::Int) || var.canConvert(QVariant::String) || var.canConvert(QVariant::Bool) ) {
                            editor = new LineEdit(this);
                        } else if ( var.canConvert( QVariant::List ) ) {
                            QList<QVariant> list = var.toList();
                            if ( list.length() > 0 ){
                                QVariant val = list[0];
                                if ( val.canConvert(QVariant::String ) ){
                                    editor = new ListEdit(this);
                                } else {
                                    Grid* grid = new Grid(this);
                                    QVariantMap valMap = val.toMap();
					                grid->setColumnHeaders(valMap.keys());
                                    editor = grid;
                                }
                            }
                            
                        } else {
                            editor = new LineEdit(this);
                        } 
                        widget = new TemplateWidget(editor, this);
                        widget->setLabel(key);
                        widget->setField(key);
                        QSize hint = widget->sizeHint();
                        widget->setGeometry(x,y,hint.width(), hint.height());
                        widget->show();
                        widgets.push_back(widget);
                        
                        tabOrder.append(qMakePair(y, widget));
                                        
                        y += widget->sizeHint().height();
                    }
                }
            
        
                saveTemplate();
            }
        }
    }
    
    /* Sort the tab order list and use it to set the tab orders on each widget */
    /*qSort(tabOrder.begin(), tabOrder.end(), widgetLocationSort);
    int i = 0;
    
    for(; i<tabOrder.size()-1; i++){
        qDebug() << tabOrder[i].second->getField();
        QWidget::setTabOrder(tabOrder[i].second->getEditor(), tabOrder[i+1].second->getEditor());
    }*/
}


void View::saveTemplate(){
    
    QList<QVariant> fields = QList<QVariant>();
	qSort(widgets.begin(), widgets.end(), widgetLocationSort);
    for(int i=0; i<widgets.size(); i++){
        QVariantMap fieldMap = QVariantMap();
        TemplateWidget* widget = widgets[i];
        QVariant var = widget->saveConfiguration();
        fields.append(var);   
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
    }catch(DocumentNotFoundException){
        QString id = "templates/"+design+"/_view/"+view;
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
    
    QString id = "templates/"+design+"/_view/"+view;
    
    Document templateDoc = couch.getDocument(database, id);
    return templateDoc;
    
}

bool View::loadDocument(QString database, QString id){
    Document doc = couch.getDocument(database, id);
    return loadDocument(doc, false);
}

bool View::loadDocument(Document doc, bool force, bool skipAttachments){
    
    if ( !force && hasChanges() ) {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes before continuing?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        
        switch( ret ) {
            case QMessageBox::Save:
                saveChanges();
                break;
            case QMessageBox::Cancel:
            default: // just for sanity
                return false;
                break;
            case QMessageBox::Discard:
                break;
        }
    }
    
    currentDoc = doc;
    
    for (int i=0; i<widgets.size(); i++) {
        widgets[i]->loadDocument(doc);
    }
    
    	
    if ( !skipAttachments ) {
        attachments->loadDocument(doc);
    }
    hasDocument = true;
    return true;
}

void View::fileAttached(QUrl url) {
    QString canonicalFilename = url.toLocalFile();
    QString filename = canonicalFilename.split("/").last();
    QFile file(canonicalFilename);
    if ( file.open(QIODevice::ReadOnly) ) {
        QString revision = couch.putAttachment(currentDoc.getSourceDatabase(), currentDoc.getId(), currentDoc.getRevision(), filename, &file);
        file.close();
        currentDoc.setRevision(revision);
        loadDocument(currentDoc, true, true);
        emit documentUpdated(currentDoc);
    
    }
}

void View::removeAllWidgets(){
    for (int i=0; i<widgets.size(); i++) {
        widgets[i]->hide();
        widgets[i]->deleteLater();
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

void View::removeWidget(TemplateWidget *widget){
    
    for (int i=0; i<widgets.size(); i++) {
        if ( widgets[i] == widget ){
            widgets.erase(widgets.begin()+i);
        }
    }
    
    /*delete widget;
    */
    widget->hide();
    widget->deleteLater();
    
}

void View::beginEditing(){
    if (isEditing) {
        isEditing = false;
        editButton->setText("Edit Template");
        addFieldButton->setVisible(false);
        newButton->setVisible(true);
        saveButton->setVisible(true);
        deleteButton->setVisible(true);
        
        for (int i=0; i<widgets.size(); i++) {
            widgets[i]->stopEditing();
        }
        
        saveTemplate();
		toolbar->setVisible(false);
	    if ( selectedWidget != NULL ){
	 		selectedWidget->getEditor()->removeConfigurationAction(toolbar);
		}
		selectedWidget = NULL;
        
    } else {
        isEditing = true;
     	buildHotSpots();
        editButton->setText("Stop Editing");
        addFieldButton->setVisible(true);
        newButton->setVisible(false);
        saveButton->setVisible(false);
        deleteButton->setVisible(false);
        for (int i=0; i<widgets.size(); i++) {
            widgets[i]->beginEditing();
        }
        
    }
	update();
}

void View::addField(){
    TemplateWidget *widget = new TemplateWidget(new LineEdit(this), this);
    widget->setLabel("New Field");
    
    QSize hint = widget->sizeHint();
    widget->setGeometry(0,0,hint.width(), hint.height());
    widget->show();
    widget->beginEditing();
    widgets.push_back(widget);
    
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
     
     /*for (unsigned int i=0; i<hotSpots.size(); i++) {
         painter.fillRect(hotSpots[i].rect(), Qt::yellow);
     }*/

	
	if ( selectedWidget != NULL && isEditing && !activeDragging && !activeAction ){
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setColor(Qt::darkGray);
     
        painter.setPen(pen);
       
		painter.drawRect(selectedWidget->geometry());	
		QRect rect = selectedWidget->geometry();
		QPoint bottomRight = rect.bottomRight();
		QPoint center = rect.center();
		QPoint right, bottom;
		right.setX(rect.x() + rect.width());
		right.setY(center.y());
		bottom.setX(center.x());
		bottom.setY(rect.y()+rect.height());
		if ( selectedWidget->allowResizeHeight() && selectedWidget->allowResizeWidth() ) {
			painter.fillRect(bottomRight.x()-1,bottomRight.y()-1, 6, 6, Qt::darkGray);
			painter.fillRect(right.x()-1,right.y()-1, 6, 6, Qt::darkGray);
			painter.fillRect(bottom.x()-1,bottom.y()-1, 6, 6, Qt::darkGray);
		} else if ( selectedWidget->allowResizeWidth() ){
			painter.fillRect(right.x()-1,right.y()-1, 6, 6, Qt::darkGray);
		} else if ( selectedWidget->allowResizeHeight() ) {
			painter.fillRect(bottom.x()-1,bottom.y()-1, 6, 6, Qt::darkGray);
		}
	}
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
	 if ( selectedWidget != NULL ){
	 	selectedWidget->getEditor()->removeConfigurationAction(toolbar);
	 }
	 selectedWidget = templWidget;
	 selectedWidget->getEditor()->addConfigurationAction(toolbar);
	 toolbar->resize(toolbar->sizeHint());
	 toolbar->move(selectedWidget->width()+selectedWidget->x()-toolbar->width(), selectedWidget->y()-toolbar->height());
	 toolbar->setVisible(true);
     activeWidget->setCursor(Qt::ClosedHandCursor);
	 activeWidget->raise();
	 activeDragging = true;
	 toolbar->raise();
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
        toolbar->move(selectedWidget->width()+selectedWidget->x()-toolbar->width(), selectedWidget->y()-toolbar->height());
        return;
    } else if ( activeAction && action == RESIZE_WIDTH ) {
        int width = qMax(actionWidget->minimumWidth(), event->pos().x() - actionWidget->x());
        actionWidget->setGeometry(actionWidget->x(), actionWidget->y(), width, actionWidget->height() );
        toolbar->move(selectedWidget->width()+selectedWidget->x()-toolbar->width(), selectedWidget->y()-toolbar->height());
        return;
    } else if ( activeAction && action == RESIZE_HEIGHT ) {
        int height = qMax(actionWidget->minimumHeight(), event->pos().y() - actionWidget->y());
        actionWidget->setGeometry(actionWidget->x(), actionWidget->y(), actionWidget->width(), height );
        return;
    }
        
    
    if ( activeDragging ) {
        activeWidget->move(event->pos().x()-offsetX, event->pos().y()-offsetY);
        toolbar->move(selectedWidget->width()+selectedWidget->x()-toolbar->width(), selectedWidget->y()-toolbar->height());
        
        
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
                    toolbar->move(selectedWidget->width()+selectedWidget->x()-toolbar->width(), selectedWidget->y()-toolbar->height());
                    hintX = hint;
                }
            
                hint = widgets[i]->getTopAlignmentHint() + widgets[i]->y();
                if ( abs( hint - (event->pos().y() + activeWidgetHintY) + offsetY ) < THRESHOLD ) {
                    activeWidget->move(activeWidget->x(), hint - activeWidgetHintY);
                    toolbar->move(selectedWidget->width()+selectedWidget->x()-toolbar->width(), selectedWidget->y()-toolbar->height());
                    hintY = hint;
                }
            
            /* Check for margins */
                if ( hintX > -1 ) {
                    if ( abs(event->pos().y() - widgets[i]->y() - widgets[i]->height() - offsetY) < THRESHOLD ) {
                        activeWidget->move(activeWidget->x(), widgets[i]->y() + widgets[i]->height());
                        toolbar->move(selectedWidget->width()+selectedWidget->x()-toolbar->width(), selectedWidget->y()-toolbar->height());
                    }
           
                    if ( abs(event->pos().y() - offsetY - widgets[i]->y() + activeWidget->height() ) < THRESHOLD ) {
                        activeWidget->move(activeWidget->x(), widgets[i]->y() - activeWidget->height());
                        toolbar->move(selectedWidget->width()+selectedWidget->x()-toolbar->width(), selectedWidget->y()-toolbar->height());
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



void View::changeEditorDate(){
	selectedWidget->changeEditor(new DateEdit(this));
}

void View::changeEditorTextarea(){
	selectedWidget->changeEditor(new TextEdit(this));
}

void View::changeEditorText(){
	selectedWidget->changeEditor(new LineEdit(this));
}

void View::changeEditorComboBox(){
	selectedWidget->changeEditor(new Combo(this));
}


void View::changeEditorList(){
	selectedWidget->changeEditor(new ListEdit(this));
}

void View::changeEditorGrid(){
	selectedWidget->changeEditor(new Grid(this));
}

void View::showChangeEditorMenu(){
    changeEditorMenu->move(mapToGlobal(QPoint(toolbar->x(), toolbar->y()+toolbar->height())));
    changeEditorMenu->show();
}

void View::deleteWidget(){
	removeWidget(selectedWidget);
	selectedWidget = NULL;
	toolbar->setVisible(false);
	update();
}





