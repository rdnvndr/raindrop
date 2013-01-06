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
