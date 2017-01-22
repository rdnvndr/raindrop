include(../tst_plugin.pri)
CONFIG += plugin
TEMPLATE = lib
DEFINES +=  TST_PLUGIN2_LIBRARY

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "TST_PLUGIN2"
QMAKE_TARGET_DESCRIPTION = "TST_PLUGIN2"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

# Input
HEADERS   += tst_plugin2.h tst_plugin2global.h itst_plugin2.h
SOURCES   += tst_plugin2.cpp
RESOURCES += tst_plugin2.qrc

