VERSION = 1.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "UndoStack"
QMAKE_TARGET_DESCRIPTION = "Undo Stack Plugin"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  UNDOSTACK_LIBRARY
QT += widgets

# Input
HEADERS   += undostack.h undostackglobal.h
SOURCES   += undostack.cpp
RESOURCES += undostack.qrc
