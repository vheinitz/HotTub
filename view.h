

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
#include "qcouch/qcouch.h"

 class QDragEnterEvent;
 class QDropEvent;

using namespace std;

 class View : public QWidget
 {
	Q_OBJECT
			
 public:
     View( QCouch& couch, QWidget *parent = 0);
     void setDatabase(QString);
     void loadTemplate(QString, QString);
     bool loadDocument(Document, bool force = false);
     void reset();
     void clear();
     bool hasChanges();
     void saveChanges();
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
     void saveTemplate();
private slots:
     void beginEditing();
     void addField();
     void hideAttachments();
     void showAttachments();
     void fileAttached(QUrl url);
     void saveDocument();
     void deleteDocument();
     void newDocument();

     void showChangeEditorMenu();
    
     void changeEditorDate();
     void changeEditorTextarea();
     void changeEditorText();
     void changeEditorComboBox();
     void changeEditorList();
     void changeEditorGrid();
     
     void deleteWidget();
signals:
     void documentDeleted(Document& doc);
     void documentUpdated(Document& doc);
     void documentAdded(Document& doc);
     
 private:
     void buildHotSpots();
     Document findTemplate();
     void removeWidget(TemplateWidget *);

	 QToolBar *toolbar;
     QMenu *changeEditorMenu;
     
     QCouch &couch;
     QString database;
     QString design;
     QString view;
     vector<int> leftAlignmentHints;
     vector<Hotspot> hotSpots;
     QList<TemplateWidget*> widgets;
 
     QGridLayout *documentLayout;
     
     TemplateWidget* activeWidget;
     QWidget* actionWidget;

	 TemplateWidget* selectedWidget;
     
     QPushButton *editButton;
     QPushButton *addFieldButton;
     QPushButton *newButton;
     QPushButton *saveButton;
     QPushButton *deleteButton;
     
     
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
     Document currentDoc;
     bool hasDocument;

 };


 #endif
