include(../../subdir.pri)
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
LIBS += -lplugin
SOURCES += tst_pluginmanager.cpp \
    pluginloadlog.cpp

HEADERS += \
    pluginloadlog.h
