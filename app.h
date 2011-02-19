

#ifndef __APP_H
#define __APP_H

#include <QtGui>


class Application : public QApplication {
 
public:
    Application(int argc, char *argv[]);
    bool notify(QObject *, QEvent *);
    
};





#endif