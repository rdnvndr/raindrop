LIBS += -lplugin -lmdiextarea
include(../plugins.pri)

TARGET =
DEPENDPATH += .
INCLUDEPATH += .
DEFINES += MAINWINDOW_LIBRARY

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
