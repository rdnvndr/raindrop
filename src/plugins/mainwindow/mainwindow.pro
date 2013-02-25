LIBS += -lplugin -lmdiextarea
include(../plugins.pri)

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
    listview.h \
    actionprop.h \
    toolbar.h
SOURCES += \
           mainwindow.cpp \
    menubar.cpp \
    mainwindowoptions.cpp \
    actiongroupmodel.cpp \
    mimedataobject.cpp \
    menu.cpp \
    listview.cpp \
    actionprop.cpp \
    toolbar.cpp

FORMS += \
           mainwindow.ui \
    mainwindowoptions.ui \
    actionprop.ui

RESOURCES += \
    mainwindow.qrc
