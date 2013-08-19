LIBS += -lplugin
include(../subdir.pri)

CONFIG += plugin
TEMPLATE = lib

DEPENDPATH += .
INCLUDEPATH += .
QT += widgets


win32 {
   CONFIG (debug, debug|release) {
      DESTDIR = $$PWD/tst_pluginmanager/debug/plugins
   } else {
      DESTDIR = $$PWD/tst_pluginmanager/release/plugins
   }
}
unix {
   DESTDIR = $$PWD/tst_pluginmanager/plugins
}