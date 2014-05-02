VERSION = 1.0.0
QMAKE_TARGET_COMPANY = RTPTechGroup
QMAKE_TARGET_PRODUCT = Raindrop
QMAKE_TARGET_DESCRIPTION = TreeComboBox Library
QMAKE_TARGET_COPYRIGHT = Copyright (C) RTPTechGroup

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
