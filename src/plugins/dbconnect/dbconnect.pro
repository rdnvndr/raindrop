LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  DBCONNECT_LIBRARY
QT += widgets

# Input
HEADERS   += dbconnect.h dbconnectglobal.h
SOURCES   += dbconnect.cpp
RESOURCES += dbconnect.qrc

