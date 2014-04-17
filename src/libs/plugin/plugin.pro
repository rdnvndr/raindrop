include(../libs.pri)
DEFINES += PLUGIN_LIBRARY

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = RTPTechGroup
QMAKE_TARGET_PRODUCT = Raindrop
QMAKE_TARGET_DESCRIPTION = Plugin Library
QMAKE_TARGET_COPYRIGHT = Copyright (C) RTPTechGroup


greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

HEADERS += \
    pluginmanager.h \
    iplugin.h \
    pluginglobal.h

SOURCES += \
    pluginmanager.cpp \
    iplugin.cpp
