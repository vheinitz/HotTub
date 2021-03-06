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

#include "viewconfig.h"
#include <QtGui>
#include <QDebug>


ViewConfig::ViewConfig() : QDialog(){

	QVBoxLayout *mainLayout = new QVBoxLayout;

	QHBoxLayout *layout = new QHBoxLayout;
	allColumnsListWidget = new QListWidget;
	currentColumnsListWidget = new QListWidget;

	connect(allColumnsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(addDoubleClicked(QListWidgetItem *)));
	connect(currentColumnsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(removeDoubleClicked(QListWidgetItem *)));

	QVBoxLayout *operationsLayout = new QVBoxLayout;
	QPushButton *add = new QPushButton(">");
	QPushButton *addAll = new QPushButton(">>");
	QPushButton *remove = new QPushButton("<");
	QPushButton *removeAll = new QPushButton("<<");

	connect(add, SIGNAL(clicked()), this, SLOT(addClicked()));
	connect(addAll, SIGNAL(clicked()), this, SLOT(addAllClicked()));
	connect(remove, SIGNAL(clicked()), this, SLOT(removeClicked()));
	connect(removeAll, SIGNAL(clicked()), this, SLOT(removeAllClicked()));

	operationsLayout->addStretch();
	operationsLayout->addWidget(add);
	operationsLayout->addWidget(addAll);
	operationsLayout->addWidget(remove);
	operationsLayout->addWidget(removeAll);
	operationsLayout->addStretch();

	layout->addWidget(allColumnsListWidget);
	layout->addLayout(operationsLayout);
	layout->addWidget(currentColumnsListWidget);
	
	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	QPushButton* ok = new QPushButton(tr("Ok"));
	QPushButton* cancel = new QPushButton(tr("Cancel"));

	ok->setDefault(true);
	connect(ok, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);
	
	mainLayout->addLayout(layout);
	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);

}

void ViewConfig::okClicked(){
	emit accepted();
	close();
}

void ViewConfig::cancelClicked(){
	emit rejected();
	close();
}

void ViewConfig::addDoubleClicked(QListWidgetItem * item){
	Q_UNUSED(item);
	addSelected();
}

void ViewConfig::removeDoubleClicked(QListWidgetItem * item){
	Q_UNUSED(item);
	removeSelected();
}

void ViewConfig::addColumn(QString text){
	allColumnsListWidget->addItem(text);
}

void ViewConfig::addCurrentColumn(QString text){
	currentColumnsListWidget->addItem(text);
}

QStringList ViewConfig::getCurrentColumns(){
	QStringList currentColumns;
	for(int i=0; i<currentColumnsListWidget->count(); i++){
		QListWidgetItem *item = currentColumnsListWidget->item(i);
		currentColumns << item->text();	
	}
	return currentColumns;
}


void ViewConfig::addSelected(){
	int row = allColumnsListWidget->currentRow();
	if ( row >= 0 ){
		QListWidgetItem *item = allColumnsListWidget->takeItem(row);
		currentColumnsListWidget->addItem(item);
	}	
}

void ViewConfig::removeSelected(){
	int row = currentColumnsListWidget->currentRow();
	if ( row >= 0 ){
		QListWidgetItem *item = currentColumnsListWidget->takeItem(row);
		allColumnsListWidget->addItem(item);
	}	
}


void ViewConfig::addClicked(){
	addSelected();
}

void ViewConfig::addAllClicked(){
	while ( allColumnsListWidget->count() > 0 ){
		QListWidgetItem *item = allColumnsListWidget->takeItem(0);
		currentColumnsListWidget->addItem(item);
	}
}

void ViewConfig::removeClicked(){
	removeSelected();
}

void ViewConfig::removeAllClicked(){
	while ( currentColumnsListWidget->count() > 0 ){
		QListWidgetItem *item = currentColumnsListWidget->takeItem(0);
		allColumnsListWidget->addItem(item);
	}
}

