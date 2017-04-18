include(../../manual.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_tilingframe
TEMPLATE = app

LIBS += -ltilingframe

HEADERS   = *.h
SOURCES   = *.cpp
RESOURCES = *.qrc
FORMS     = *.ui
