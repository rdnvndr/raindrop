VERSION = 1.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "PgDatabaseModel"
QMAKE_TARGET_DESCRIPTION = "Реализация модели данных для PostgreSql"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  PGDATABASEMODEL_LIBRARY
QT += widgets

# Input
HEADERS   += pgdatabasemodel.h pgdatabasemodelglobal.h
SOURCES   += pgdatabasemodel.cpp
RESOURCES += pgdatabasemodel.qrc
