VERSION = 1.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Модель базы данных"
QMAKE_TARGET_DESCRIPTION = "Модель базы данных"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  DATABASEMODELMANAGER_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets sql
}


# Input
HEADERS   = *.h
SOURCES   = *.cpp
RESOURCES = *.qrc
