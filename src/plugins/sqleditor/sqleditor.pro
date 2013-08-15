LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  SQLEDITOR_LIBRARY
QT += widgets sql

# Input
HEADERS   += sqleditor.h sqleditorglobal.h \
    querysqlwidget.h
SOURCES   += sqleditor.cpp \
    querysqlwidget.cpp
RESOURCES += sqleditor.qrc 

FORMS += \
    querysqlwidget.ui

