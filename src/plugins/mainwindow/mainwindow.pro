LIBS += -lplugin -lmdiextarea
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES += MAINWINDOW_LIBRARY
QT += widgets

# Input
HEADERS += mainwindowplug.h \
           mainwindow.h \
    mainwindowglobal.h
SOURCES += mainwindowplug.cpp \
           mainwindow.cpp

FORMS += \
           mainwindow.ui

RESOURCES += \
    mainwindow.qrc
