 HEADERS     = templatewidget.h \
               dragwidget.h \
               textedit.h
 RESOURCES   = draggabletext.qrc
 SOURCES     = templatewidget.cpp \
               dragwidget.cpp \
               textedit.cpp \
               main.cpp

 # install
 target.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggabletext
 sources.files = $$SOURCES $$HEADERS $$RESOURCES *.txt *.pro
 sources.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggabletext
 INSTALLS += target sources

 symbian {
     TARGET.UID3 = 0xA000CF64
     include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 }
