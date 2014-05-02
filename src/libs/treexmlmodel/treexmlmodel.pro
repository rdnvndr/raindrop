VERSION = 1.0.0
QMAKE_TARGET_COMPANY = RTPTechGroup
QMAKE_TARGET_PRODUCT = Raindrop
QMAKE_TARGET_DESCRIPTION = TreeXmlModel Library
QMAKE_TARGET_COPYRIGHT = Copyright (C) RTPTechGroup

include(../libs.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
QT += xml
DEFINES += XMLMODEL_LIBRARY
HEADERS += \
    tagxmlitem.h \
    tablexmlproxymodel.h \
    treexmlmodelglobal.h \
    modifyproxymodel.h \
    treexmlhashmodel.h \
    treexmlmodel.h \
    mimedataindex.h

SOURCES += \ 
    tagxmlitem.cpp \
    tablexmlproxymodel.cpp \
    modifyproxymodel.cpp \
    treexmlhashmodel.cpp \
    treexmlmodel.cpp \
    mimedataindex.cpp

