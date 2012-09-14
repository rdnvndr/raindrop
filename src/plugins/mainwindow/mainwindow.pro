#-------------------------------------------------
#
# Project created by QtCreator 2012-07-27T18:30:34
#
#-------------------------------------------------
LIBS += -lextensionsystem -lcommon
include(../plugins.pri)

TARGET = mainwindows
TEMPLATE = lib

DEFINES += MAINWINDOWS_LIBRARY

SOURCES += \
    mainwindow.cpp \
    mainwindowplugin.cpp

HEADERS +=\
    mainwindow.h \
    mainwindowplugin.h \
    mainwindowglobal.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    mainwindow.qrc
