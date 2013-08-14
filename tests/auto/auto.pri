include(../tests.pri)
SHAREDIR = $$PWD/shared
INCLUDEPATH += $$PWD $$SHAREDIR
QT += testlib
CONFIG += qt warn_on console depend_includepath testcase
CONFIG -= app_bundle
DEFINES -= QT_NO_CAST_FROM_ASCII