VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "DockWidget Library"
QMAKE_TARGET_DESCRIPTION = "DockWidget Library"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

include(../libs.pri)

DEFINES += DOCKWIDGET_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
# Input
HEADERS   = *.h
SOURCES   = *.cpp
