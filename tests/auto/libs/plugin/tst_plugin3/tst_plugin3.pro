include(../tst_plugin.pri)
DEFINES +=  TST_PLUGIN3_LIBRARY

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "TST_PLUGIN3"
QMAKE_TARGET_DESCRIPTION = "TST_PLUGIN3"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

# Input
HEADERS   += tst_plugin3.h tst_plugin3global.h itst_plugin3.h
SOURCES   += tst_plugin3.cpp
RESOURCES += tst_plugin3.qrc

