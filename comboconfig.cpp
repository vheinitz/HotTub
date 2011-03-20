

#include "comboconfig.h"


ComboConfigurationDialog::ComboConfigurationDialog() {
    
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
    
    QPushButton* deleteButton = new QPushButton(tr("Delete"));
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
    
}


void ComboConfigurationDialog::addItem(){
    list << edit->text();
    model->setStringList(list);
    edit->clear();
}


void ComboConfigurationDialog::okButtonClicked(){
    emit accepted();
    close();
}

void ComboConfigurationDialog::cancelButtonClicked(){
    emit rejected();
    close();
}

QStringList ComboConfigurationDialog::stringList(){
    return list;
}


