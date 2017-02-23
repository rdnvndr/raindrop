VERSION = 1.0.0
QMAKE_TARGET_COMPANY =     "RTPTechGroup"
QMAKE_TARGET_PRODUCT =     "Подключение к БД"
QMAKE_TARGET_DESCRIPTION = "DbConnect Plugin"
QMAKE_TARGET_COPYRIGHT =   "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  DBCONNECT_LIBRARY
QT += widgets
QT += sql

# Input
HEADERS   += dbconnect.h dbconnectglobal.h \
    dialogconnect.h threadconnect.h
SOURCES   += dbconnect.cpp \
    dialogconnect.cpp threadconnect.cpp
RESOURCES += dbconnect.qrc

FORMS += \
    dialogconnect.ui

