
#include <QApplication>
#include "view.h"

#include <QtGui>
#include "list.h"
#include "view.h"


class MainWindow : public QWidget {
public:
    MainWindow(QWidget *parent = 0);
    
};

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setMinimumSize(1200, 600);
    
    View *view = new View(this);
    List *list = new List(this);
    
    view->setMinimumWidth(600);
    list->setMinimumWidth(600);
    
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(5,5,5,5);
    layout->addWidget(list, 0, 0);
    layout->addWidget(view, 0, 1);
    setLayout(layout);
}




int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    //View view;
    //view.show();
    
    return app.exec();
}
