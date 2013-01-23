LIBS += -lplugin -lmdiextarea
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES += MAINWINDOW_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

# Input
HEADERS += \
           mainwindow.h \
    mainwindowglobal.h \
    menubar.h
SOURCES += \
           mainwindow.cpp \
    menubar.cpp

FORMS += \
           mainwindow.ui

RESOURCES += \
    mainwindow.qrc
