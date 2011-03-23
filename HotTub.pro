######################################################################
# Automatically generated by qmake (2.01a) Thu Mar 10 20:09:30 2011
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . editors qcouch qcouch/qjson
INCLUDEPATH += . editors qcouch qcouch/qjson

CONFIG += debug

QT += network

# Input
HEADERS += action.h \
           app.h \
           attachments.h \
           editor.h \
           hotspot.h \
           list.h \
           main.h \
           model.h \
           templatewidget.h \
           view.h \
           editors/comboconfig.h \
           editors/lineedit.h \
           editors/textedit.h \
           editors/combo.h \
           editors/dateedit.h \
           editors/listedit.h \
           editors/grid.h \
           editors/gridmodel.h \
           qcouch/document.h \
           qcouch/qcouch.h \
           qcouch/qjson/json_parser.hh \
           qcouch/qjson/json_scanner.h \
           qcouch/qjson/location.hh \
           qcouch/qjson/parser.h \
           qcouch/qjson/parser_p.h \
           qcouch/qjson/parserrunnable.h \
           qcouch/qjson/position.hh \
           qcouch/qjson/qjson_debug.h \
           qcouch/qjson/qjson_export.h \
           qcouch/qjson/qobjecthelper.h \
           qcouch/qjson/serializer.h \
           qcouch/qjson/serializerrunnable.h \
           qcouch/qjson/stack.hh
SOURCES += app.cpp \
           attachments.cpp \
           editor.cpp \
           hotspot.cpp \
           list.cpp \
           main.cpp \
           model.cpp \
           templatewidget.cpp \
           view.cpp \
           editors/listedit.cpp \
           editors/comboconfig.cpp \
           editors/combo.cpp \
           editors/dateedit.cpp \
           editors/textedit.cpp \
           editors/lineedit.cpp \
           editors/grid.cpp \
           editors/gridmodel.cpp \
           qcouch/document.cpp \
           qcouch/qcouch.cpp \
           qcouch/qjson/json_parser.cc \
           qcouch/qjson/json_scanner.cpp \
           qcouch/qjson/parser.cpp \
           qcouch/qjson/parserrunnable.cpp \
           qcouch/qjson/qobjecthelper.cpp \
           qcouch/qjson/serializer.cpp \
           qcouch/qjson/serializerrunnable.cpp


