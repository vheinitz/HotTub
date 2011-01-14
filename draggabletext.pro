 HEADERS     = templatewidget.h \
               dragwidget.h \
               lineedit.h \
               textedit.h \
               hotspot.h \
               action.h \
               editor.h \
               attachments.h \
               dateedit.h
               combo.h 
 RESOURCES   = draggabletext.qrc
 SOURCES     = templatewidget.cpp \
               dragwidget.cpp \
               textedit.cpp \
               lineedit.cpp \
               dateedit.cpp \
               combo.cpp \
               hotspot.cpp \
               editor.cpp \
               attachments.cpp \
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
