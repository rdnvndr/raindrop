VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Дерево команд"
QMAKE_TARGET_DESCRIPTION = "TreeDockWidget Plugin"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lfiltertreewidget -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .

# Input

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

DEFINES += TREEDOCKWIDGET_LIBRARY


HEADERS   = *.h
SOURCES   = *.cpp
RESOURCES = *.qrc
FORMS     = *.ui
