/*
 This file is part of Hot Tub, a project to help interactions with Apache CouchDB
 Copyright 2011 Matt Jennings <jennmat@gmail.com>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3.0 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <Qt>
#include <QtGui>
#include <iostream>
#include "templatewidget.h"
#include "editor.h"
#include "dateedit.h"
#include "lineedit.h"
#include "textedit.h"
#include "combo.h"
#include "listedit.h"
#include "grid.h"


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
	widget->setMaximumWidth(100);
	widget->setMinimumWidth(100);
    widget->setLayout(stack);
    
    layout->addWidget(widget);
    edit->setMargins(0, 0, 0, 0);
    layout->addWidget(edit);
    layout->setContentsMargins(5,MARGIN,5,0);
    QWidget::setLayout(layout);
    
    setFocusProxy(editor);
    
    //edit->configurationAction(toolbar);
    
    
    connect(labelEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateLabel(const QString&)));
    
 
    isEditing = false;
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
    
}


void TemplateWidget::enterEvent(QEvent *event){
    Q_UNUSED(event);
    
    if ( isEditing ) {
    }
}


void TemplateWidget::leaveEvent(QEvent *event){
    Q_UNUSED(event);
    
    if ( isEditing ) {
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

bool TemplateWidget::hasChanges(){
    return editor->hasChanges();
}


void TemplateWidget::changeEditor(Editor *newEditor){
    layout->removeWidget(editor);
    editor->deleteLater();
	editor = newEditor;
    editor->setMargins(0,0,0,0);
   	editor->enterEditMode();
    layout->addWidget(editor);
    setFocusProxy(editor);
    resize(sizeHint());
}

Editor* TemplateWidget::getEditor(){
	return editor;	
}

