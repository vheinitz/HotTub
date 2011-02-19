######################################################################
# Automatically generated by qmake (2.01a) Sun Feb 6 19:40:43 2011
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

macx {
    INCLUDEPATH += /opt/local/include
    INCLUDEPATH += ../couchdbpp/include
    
    LIBS += -L../couchdbpp/src -lcouchdb++
}

CONFIG += DEBUG

# Input
HEADERS += action.h \
           attachments.h \
           combo.h \
           dateedit.h \
           editor.h \
           hotspot.h \
           lineedit.h \
           list.h \
           main.h \
           templatewidget.h \
           textedit.h \
           view.h \
           model.h \
           app.h 
SOURCES += attachments.cpp \
           combo.cpp \
           dateedit.cpp \
           editor.cpp \
           hotspot.cpp \
           lineedit.cpp \
           list.cpp \
           main.cpp \
           templatewidget.cpp \
           textedit.cpp \
           view.cpp \
           model.cpp \
           app.cpp