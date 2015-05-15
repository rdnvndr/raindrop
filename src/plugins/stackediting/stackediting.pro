VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Стек редактирования"
QMAKE_TARGET_DESCRIPTION = "Stack Editing Plugin"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  STACKEDITING_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}


# Input
HEADERS   += \
    stackeditingglobal.h \
    stackediting.h
SOURCES   += \
    stackediting.cpp
RESOURCES += \
    stackediting.qrc

