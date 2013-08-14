LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  DBCONNECT_LIBRARY
QT += widgets
QT += sql

# Input
HEADERS   += dbconnect.h dbconnectglobal.h \
    dialogconnect.h
SOURCES   += dbconnect.cpp \
    dialogconnect.cpp
RESOURCES += dbconnect.qrc

FORMS += \
    dialogconnect.ui

