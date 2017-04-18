VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Главное окно"
QMAKE_TARGET_DESCRIPTION = "MainWindow Plugin"
QMAKE_TARGET_COPYRIGHT =   "Copyright (C) RTPTechGroup"

LIBS += -lplugin -lmdiextarea -lpushbuttonimage
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES += MAINWINDOW_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

# Input
HEADERS   = *.h
SOURCES   = *.cpp
RESOURCES = *.qrc
FORMS     = *.ui
