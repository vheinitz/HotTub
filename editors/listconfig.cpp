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

#include "listconfig.h"


ListConfigurationDialog::ListConfigurationDialog() {
    
    model = new QStringListModel();
        
    model->setStringList(list);
    
    QListView* listView = new QListView();
    listView->setModel(model);
    
    QGridLayout* layout = new QGridLayout;
    layout->setSpacing(7);
    layout->addWidget(listView, 0, 0);
    
    QHBoxLayout *addItemLayout = new QHBoxLayout;
    edit = new QLineEdit;
    QPushButton* button = new QPushButton(tr("Add"));
    
    QPushButton* okButton = new QPushButton(tr("Ok"));
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    
    addItemLayout->addWidget(edit);
    addItemLayout->addWidget(button);
    
    layout->addLayout(addItemLayout, 1, 0);
    
    
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout, 2, 0);
    setLayout(layout);
    
    setMinimumSize(QSize(250, 250));
    
    connect(button, SIGNAL(clicked()), this, SLOT(addItem()));    

    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
    
    connect(listView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));
}

void ListConfigurationDialog::doubleClicked(const QModelIndex &index){
    list.removeAt(index.row());
    model->setStringList(list);
}

void ListConfigurationDialog::addAll(QStringList& _list){
    list.clear();
    list << _list;
    model->setStringList(list);
}


void ListConfigurationDialog::addItem(){
    list << edit->text();
    model->setStringList(list);
    edit->clear();
}


void ListConfigurationDialog::okButtonClicked(){
    emit accepted();
    close();
}

void ListConfigurationDialog::cancelButtonClicked(){
    emit rejected();
    close();
}

QStringList ListConfigurationDialog::stringList(){
    return list;
}


