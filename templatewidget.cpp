
#include <Qt>
#include <QtGui>
#include <iostream>
#include "templatewidget.h"
#include "editor.h"
#include "dateedit.h"
#include "lineedit.h"
#include "textedit.h"
#include "combo.h"


using namespace std;

TemplateWidget::TemplateWidget(Editor* edit, QWidget *parent)
     : QWidget(parent)
{
    layout = new QHBoxLayout;
    stack = new QStackedLayout;
	label = new QLabel;
    labelEdit = new QLineEdit;
    editor = edit;
    
    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->addWidget(label);
    labelLayout->addStretch();
    
    QWidget *labelContainer = new QWidget;
    labelLayout->setContentsMargins(0,MARGIN,0,0);
    labelContainer->setLayout(labelLayout);
    
    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->setContentsMargins(0,0,0,0);
    editLayout->addWidget(labelEdit);
    editLayout->addStretch();
    
    QWidget *editContainer = new QWidget;
    editContainer->setLayout(editLayout);
    stack->setContentsMargins(0,0,0,0);
    stack->addWidget(labelContainer);
    stack->addWidget(editContainer);

    
    QWidget *widget = new QWidget;
    if ( edit->type() == "combo" ) {
        /* TODO: Fix this fudge factor */
        widget->setMaximumWidth(80);
        widget->setMinimumWidth(80);
    } else {
        widget->setMaximumWidth(100);
        widget->setMinimumWidth(100);
    }    
    widget->setLayout(stack);
    
    layout->addWidget(widget);
    edit->setMargins(0, 0, 0, 0);
    layout->addWidget(edit);
    layout->setContentsMargins(5,MARGIN,5,0);
    QWidget::setLayout(layout);
    
    
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
    
    edit->configurationAction(toolbar);
    
    changeEditorMenu = new QMenu(toolbar);
    
    QAction *textAction = new QAction(changeEditorMenu);
    textAction->setText("Text");
    
    QAction *dateAction = new QAction(changeEditorMenu);
    dateAction->setText("Date");
    
    QAction *areaAction = new QAction(changeEditorMenu);
    areaAction->setText("Text Area");
    
    QAction *comboAction = new QAction(changeEditorMenu);
    comboAction->setText("Combo Box");
    
    
    changeEditorMenu->addAction(textAction);
    changeEditorMenu->addAction(dateAction);
    changeEditorMenu->addAction(areaAction);
    changeEditorMenu->addAction(comboAction);
    
    
    connect(labelEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateLabel(const QString&)));
    
    connect(textAction, SIGNAL(triggered()), this, SLOT(changeEditorText()));
    connect(dateAction, SIGNAL(triggered()), this, SLOT(changeEditorDate()));
    connect(areaAction, SIGNAL(triggered()), this, SLOT(changeEditorTextarea()));
    connect(comboAction, SIGNAL(triggered()), this, SLOT(changeEditorComboBox()));
    
    
 
    isEditing = false;
}

void TemplateWidget::changeEditorDate(){
    layout->removeWidget(editor);
    delete editor;
    editor = new DateEdit(this);
    editor->setMargins(0, 0, 0, 0);
    editor->enterEditMode();
    editor->configurationAction(toolbar);
    layout->addWidget(editor);
    toolbar->raise();
    resize(sizeHint());
}

void TemplateWidget::changeEditorTextarea(){
    layout->removeWidget(editor);
    delete editor;
    editor = new TextEdit(this);
    editor->setMargins(0,0,0,0);
    editor->enterEditMode();
    editor->configurationAction(toolbar);
    layout->addWidget(editor);
    toolbar->raise();
    resize(sizeHint());
}

void TemplateWidget::changeEditorText(){
    layout->removeWidget(editor);
    delete editor;
    editor = new LineEdit(this);
    editor->setMargins(0,0,0,0);
    editor->enterEditMode();
    editor->configurationAction(toolbar);
    layout->addWidget(editor);
    toolbar->raise();
    resize(sizeHint());
}

void TemplateWidget::changeEditorComboBox(){
    layout->removeWidget(editor);
    delete editor;
    editor = new Combo(this);
    editor->setMargins(0,0,0,0);
    editor->enterEditMode();
    layout->addWidget(editor);
    editor->configurationAction(toolbar);
    
    toolbar->raise();
    resize(sizeHint());
}



void TemplateWidget::showChangeEditorMenu(){
    changeEditorMenu->move(mapToGlobal(QPoint(toolbar->x(), toolbar->y()+toolbar->height())));
    changeEditorMenu->show();
}

void TemplateWidget::deleteWidget(){
    emit remove(this);
}

QSize TemplateWidget::sizeHint(){
    QSize editorHint = editor->sizeHint();
    return QSize(editorHint.width()+100, editorHint.height()+MARGIN);
}

int TemplateWidget::getLeftAlignmentHint(){
    return editor->x() + editor->getLeftAlignmentHint();
}

int TemplateWidget::getTopAlignmentHint(){
    return editor->y();
}

void TemplateWidget::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    //painter.fillRect(0, 0, width()-1, height()-1, Qt::red);
    //painter.drawRect(0,0,width()-1,height()-1);
    QWidget::paintEvent(event);
}

void TemplateWidget::updateLabel(const QString& text){
    setLabel(text);
}

void TemplateWidget::setLabel(const QString& text){
    label->setText(text);
    setField(text);
    labelEdit->setText(text);
}

void TemplateWidget::setCursor(Qt::CursorShape shape){
    QWidget::setCursor(shape);
    label->setCursor(shape);
    editor->setActiveCursor(shape);
}

void TemplateWidget::beginEditing(){
    setCursor(Qt::OpenHandCursor);
    stack->setCurrentIndex(1);
    editor->enterEditMode();
    isEditing = true;
}

void TemplateWidget::stopEditing(){
    setCursor(Qt::ArrowCursor);
    stack->setCurrentIndex(0);
    editor->exitEditMode();
    isEditing = false;
}

bool TemplateWidget::allowResizeWidth(){
    return editor->allowResizeWidth();
}


bool TemplateWidget::allowResizeHeight() {
    return editor->allowResizeHeight();
}


void TemplateWidget::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    
    int toolbarWidth = toolbar->width(); int toolbarHeight = toolbar->height();
    toolbar->move(width()-toolbarWidth, height()-toolbarHeight);
}


void TemplateWidget::enterEvent(QEvent *event){
    Q_UNUSED(event);
    
    if ( isEditing ) {
        toolbar->setVisible(true);
    }
}


void TemplateWidget::leaveEvent(QEvent *event){
    Q_UNUSED(event);
    
    if ( isEditing ) {
        toolbar->setVisible(false);
    }
}

void TemplateWidget::setField(const QString& _field){
    field = _field;
    editor->setField(field);
}

QString TemplateWidget::getField(){
    return field;
}


void TemplateWidget::loadDocument(Document doc){
    editor->loadDocument(doc);
}

void TemplateWidget::saveChanges(Document& doc){
    editor->saveChanges(doc);
}

void TemplateWidget::reset(){
    editor->reset();
}

QString TemplateWidget::getEditorType(){
    return editor->type();
}

QVariant TemplateWidget::saveConfiguration(){
    
    QVariantMap map;
    map["key"] = getField();
    map["x"] = x();
    map["y"] = y();
    map["width"] = width();
    map["height"] = height();
    map["editor"] = getEditorType(); 
    
    editor->saveConfiguration(map);
    
    return QVariant(map);
}


void TemplateWidget::loadConfiguration(QVariant& var){
    
    editor->loadConfiguration(var);
    
}



































