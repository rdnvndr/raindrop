include(../tst_plugin.pri)
DEFINES +=  TST_PLUGIN1_LIBRARY

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "TST_PLUGIN1"
QMAKE_TARGET_DESCRIPTION = "TST_PLUGIN1"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

# Input
HEADERS   += tst_plugin1.h tst_plugin1global.h itst_plugin1.h
SOURCES   += tst_plugin1.cpp
RESOURCES += tst_plugin1.qrc

