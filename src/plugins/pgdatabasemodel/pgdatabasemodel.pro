VERSION = 1.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Реализация модели базы данных PostgreSql"
QMAKE_TARGET_DESCRIPTION = "Реализация модели базы данных для PostgreSql"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  PGDATABASEMODEL_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets sql
}


# Input
HEADERS   += pgdatabasemodel.h pgdatabasemodelglobal.h \
    pgdatabaseclass.h \
    pgdatabaseattribute.h \
    pgdatabasecomposition.h \
    pgdatabasefilter.h \
    pgdatabaseclasses.h \
    pgdatabaseattrs.h \
    pgdatabasecomps.h \
    pgdatabasefilters.h
SOURCES   += pgdatabasemodel.cpp \
    pgdatabaseclass.cpp \
    pgdatabaseattribute.cpp \
    pgdatabasecomposition.cpp \
    pgdatabasefilter.cpp \
    pgdatabaseclasses.cpp \
    pgdatabaseattrs.cpp \
    pgdatabasecomps.cpp \
    pgdatabasefilters.cpp
RESOURCES += pgdatabasemodel.qrc
