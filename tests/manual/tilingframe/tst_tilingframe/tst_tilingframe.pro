include(../../manual.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_tilingframe
TEMPLATE = app

LIBS += -ltilingframe

SOURCES += main.cpp\
        mainwindow.cpp\ 
        tilingbutton.cpp

HEADERS  += mainwindow.h tilingbutton.h

FORMS    += mainwindow.ui

RESOURCES += \
    rsrc.qrc
