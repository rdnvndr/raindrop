include(../tst_plugin.pri)
DEFINES +=  TST_PLUGIN4_LIBRARY

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "TST_PLUGIN4"
QMAKE_TARGET_DESCRIPTION = "TST_PLUGIN4"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

# Input
HEADERS   += tst_plugin4.h tst_plugin4global.h itst_plugin4.h
SOURCES   += tst_plugin4.cpp
RESOURCES += tst_plugin4.qrc

