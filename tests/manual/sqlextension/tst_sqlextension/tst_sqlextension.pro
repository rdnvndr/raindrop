include(../../manual.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_sqlextension
TEMPLATE = app

LIBS += -lsqlextension

HEADERS   = *.h
SOURCES   = *.cpp
FORMS     = *.ui
