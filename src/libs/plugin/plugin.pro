include(../libs.pri)
DEFINES += PLUGIN_LIBRARY

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
