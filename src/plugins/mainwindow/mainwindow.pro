LIBS += -lplugin -lmdiextarea -lpushbuttonimage
include(../plugins.pri)

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = RTPTechGroup
QMAKE_TARGET_PRODUCT = Raindrop
QMAKE_TARGET_DESCRIPTION = MainWindow Plugin
QMAKE_TARGET_COPYRIGHT = Copyright (C) RTPTechGroup


DEPENDPATH += .
INCLUDEPATH += .
DEFINES += MAINWINDOW_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

# Input
HEADERS += \
           mainwindow.h \
    mainwindowglobal.h \
    menubar.h \
    mainwindowoptions.h \
    actiongroupmodel.h \
    mimedataobject.h \
    menu.h \
    actionprop.h \
    toolbar.h \
    toolbarmodel.h \
    toolbarprop.h \
    hotkeylineedit.h \
    hotkeydelegate.h \
    iconsizespinbox.h \
    actiongrouptreeview.h
SOURCES += \
           mainwindow.cpp \
    menubar.cpp \
    mainwindowoptions.cpp \
    actiongroupmodel.cpp \
    mimedataobject.cpp \
    menu.cpp \
    actionprop.cpp \
    toolbar.cpp \
    toolbarmodel.cpp \
    toolbarprop.cpp \
    hotkeylineedit.cpp \
    hotkeydelegate.cpp \
    iconsizespinbox.cpp \
    actiongrouptreeview.cpp

FORMS += \
           mainwindow.ui \
    mainwindowoptions.ui \
    actionprop.ui \
    toolbarprop.ui

RESOURCES += \
    mainwindow.qrc
