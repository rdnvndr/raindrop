VERSION = 1.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "DbModelExample"
QMAKE_TARGET_DESCRIPTION = "Пример работы с DatabaseModel"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  DBMODELEXAMPLE_LIBRARY
QT += widgets sql

# Input
HEADERS   = *.h
SOURCES   = *.cpp
RESOURCES = *.qrc
