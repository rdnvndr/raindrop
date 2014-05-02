VERSION = %Version%
QMAKE_TARGET_COMPANY = %VendorName%
QMAKE_TARGET_PRODUCT = %PluginName%
QMAKE_TARGET_DESCRIPTION = %Description%
QMAKE_TARGET_COPYRIGHT = Copyright (C) %VendorName%

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  %PluginName:u%_LIBRARY
QT += widgets

# Input
HEADERS   += %PluginName:l%.%CppHeaderSuffix% %PluginName:l%global.%CppHeaderSuffix:l%
SOURCES   += %PluginName:l%.%CppSourceSuffix%
RESOURCES += %PluginName:l%.qrc
