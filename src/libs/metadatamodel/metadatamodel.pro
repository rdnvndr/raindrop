VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "MetadataModel Library"
QMAKE_TARGET_DESCRIPTION = "MetadataModel Library"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -ltreexmlmodel
include(../libs.pri)

QT += xml
DEFINES += METADATAMODEL_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
# Input
HEADERS += metadatamodelglobal.h \
    classmodelxml.h \
    dbxmlstruct.h
SOURCES += \ 
    classmodelxml.cpp

RESOURCES += \
    metadatamodel.qrc
