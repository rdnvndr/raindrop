VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "SQL редактор"
QMAKE_TARGET_DESCRIPTION = "SqlEditor Plugin"
QMAKE_TARGET_COPYRIGHT =   "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  SQLEDITOR_LIBRARY
QT += widgets sql

# Input
HEADERS   += sqleditor.h sqleditorglobal.h \
    querysqlwidget.h \
    sqlhighlighter.h
SOURCES   += sqleditor.cpp \
    querysqlwidget.cpp \
    sqlhighlighter.cpp
RESOURCES += sqleditor.qrc 

FORMS += \
    querysqlwidget.ui

