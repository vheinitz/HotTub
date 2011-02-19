


#include <iostream>
#include "app.h"


Application::Application(int argc, char *argv[]) : QApplication(argc, argv){
    
}

bool Application::notify(QObject *obj, QEvent *event){
    try {
        return QApplication::notify(obj,event);
    }catch (std::exception &e){
        QMessageBox box;
        box.setText(e.what());
        box.exec();
        return false;
    }catch (...) {
        QMessageBox box;
        box.setText("A serious problem has occurred.");
        box.exec();
        return false;
    }
}