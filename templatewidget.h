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

#ifndef __TEMPLATEWIDGET_H
#define __TEMPLATEWIDGET_H

#include <Qt>
#include <QtGui>
#include "editor.h"

class TemplateWidget : public QWidget
{
    Q_OBJECT
	
public:
    TemplateWidget(Editor* editor, QWidget *parent);
    
    void setField(const QString &);
    QString getField();
    void loadDocument(Document doc);
    void saveChanges(Document& doc);
    void reset();
    bool hasChanges();
    void beginEditing();
    void stopEditing();
    void setCursor(Qt::CursorShape);   
    int getLeftAlignmentHint();
    int getTopAlignmentHint();
    void setLabel(const QString&);
    void paintEvent(QPaintEvent *event);
    bool allowResizeWidth();
    bool allowResizeHeight();
    void resizeEvent(QResizeEvent *event);
    QSize sizeHint();
    
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
	
	Editor* getEditor();
    
    QString getEditorType();
    QVariant saveConfiguration();
    void loadConfiguration(QVariant&);
    
	void changeEditor(Editor *);
signals:
    void remove(TemplateWidget *);
    
private slots:
    void updateLabel(const QString& text);
private:
    Editor *editor;
    QLabel *label;
    QLineEdit *labelEdit;
    QHBoxLayout *layout;
    QStackedLayout *stack;
    bool isEditing;
    QString field;
    static const int MARGIN=5;
};



#endif
