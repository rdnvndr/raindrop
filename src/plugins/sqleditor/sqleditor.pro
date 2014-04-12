LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  SQLEDITOR_LIBRARY
QT += widgets sql

# Input
HEADERS   += sqleditor.h sqleditorglobal.h \
    querysqlwidget.h \
    sqlhighlighter.h
SOURCES   += sqleditor.cpp \
    querysqlwidget.cpp \
    sqlhighlighter.cpp
RESOURCES += sqleditor.qrc 

FORMS += \
    querysqlwidget.ui

