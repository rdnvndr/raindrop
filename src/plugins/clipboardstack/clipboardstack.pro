VERSION = 1.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Clipboard Stack"
QMAKE_TARGET_DESCRIPTION = "Clipboard Stack Plugin"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  CLIPBOARDSTACK_LIBRARY
QT += widgets

# Input
HEADERS   = *.h
SOURCES   = *.cpp
RESOURCES = *.qrc
