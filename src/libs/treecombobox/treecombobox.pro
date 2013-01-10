include(../libs.pri)
DEFINES += TREECOMBOBOX_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
# Input
HEADERS += \
    treecombobox.h \
    treecomboboxglobal.h
SOURCES += \ 
    treecombobox.cpp
