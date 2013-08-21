LIBS += -lplugin
include(../subdir.pri)

CONFIG += plugin
TEMPLATE = lib

DEPENDPATH += .
INCLUDEPATH += .
QT += widgets

DESTDIR = $$DESTDIR/../tst_pluginmanager/plugins
