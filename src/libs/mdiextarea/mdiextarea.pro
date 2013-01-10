include(../libs.pri)
DEFINES += MDIEXTAREA_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
# Input
HEADERS += mdiextarea.h \
    mdiextareaglobal.h
SOURCES += mdiextarea.cpp
