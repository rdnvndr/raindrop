include(../libs.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
DEFINES += FILTERTREEWIDGET_LIBRARY
# Input
HEADERS += filtertreewidget.h \
    filtertreewidgetglobal.h
SOURCES += filtertreewidget.cpp
