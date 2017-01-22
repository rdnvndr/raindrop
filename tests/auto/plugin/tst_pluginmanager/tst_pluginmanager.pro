include(../tst_plugin.pri)
TEMPLATE = app
CONFIG += plugin
DESTDIR = $$DESTDIR/../

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
LIBS += -lplugin
SOURCES += tst_pluginmanager.cpp \
    pluginloadlog.cpp

HEADERS += \
    pluginloadlog.h
