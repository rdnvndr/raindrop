LIBS += -lfiltertreewidget -lplugin
include(../plugins.pri)

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = RTPTechGroup
QMAKE_TARGET_PRODUCT = Raindrop
QMAKE_TARGET_DESCRIPTION = TreeDockWidget Plugin
QMAKE_TARGET_COPYRIGHT = Copyright (C) RTPTechGroup


DEPENDPATH += .
INCLUDEPATH += .

# Input

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

DEFINES += TREEDOCKWIDGET_LIBRARY


HEADERS += treedockwidget.h \
           treefilterwidget.h \
    treedockwidgetglobal.h
FORMS += treefilterwidget.ui
SOURCES += treedockwidget.cpp treefilterwidget.cpp
RESOURCES += \
    treedockwidget.qrc
