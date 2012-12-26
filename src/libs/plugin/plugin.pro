include(../libs.pri)
QT += widgets
DEFINES += PLUGIN_LIBRARY
HEADERS += \
    pluginmanager.h \
    iplugin.h \
    pluginglobal.h

SOURCES += \
    pluginmanager.cpp \
    iplugin.cpp
