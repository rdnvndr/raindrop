VERSION = %Version%
QMAKE_TARGET_COMPANY = "%VendorName%"
QMAKE_TARGET_PRODUCT = "%PluginName%"
QMAKE_TARGET_DESCRIPTION = "%Description%"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) %VendorName%"

include(../libs.pri)

DEFINES += %LibName:u%_LIBRARY
QT += widgets
# Input
HEADERS += %LibName:l%.%CppHeaderSuffix% %LibName:l%global.%CppHeaderSuffix:l%
SOURCES += %LibName:l%.%CppSourceSuffix%