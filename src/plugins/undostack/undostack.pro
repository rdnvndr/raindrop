VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Стек отмены/повторения команд"
QMAKE_TARGET_DESCRIPTION = "UndoStack Plugin"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  UNDOSTACK_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}


# Input
HEADERS   += undostack.h undostackglobal.h
SOURCES   += undostack.cpp
RESOURCES += undostack.qrc

