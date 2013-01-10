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
    mainwindowglobal.h
SOURCES += \
           mainwindow.cpp

FORMS += \
           mainwindow.ui

RESOURCES += \
    mainwindow.qrc
