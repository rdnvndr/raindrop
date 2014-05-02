VERSION = 1.0.0
QMAKE_TARGET_COMPANY = RTPTechGroup
QMAKE_TARGET_PRODUCT = Raindrop
QMAKE_TARGET_DESCRIPTION = FilterTreeWidget Library
QMAKE_TARGET_COPYRIGHT = Copyright (C) RTPTechGroup

include(../libs.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
DEFINES += FILTERTREEWIDGET_LIBRARY
# Input
HEADERS += filtertreewidget.h \
    filtertreewidgetglobal.h
SOURCES += filtertreewidget.cpp
