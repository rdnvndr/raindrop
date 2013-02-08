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
    menu.h
SOURCES += \
           mainwindow.cpp \
    menubar.cpp \
    mainwindowoptions.cpp \
    actiongroupmodel.cpp \
    mimedataobject.cpp \
    menu.cpp

FORMS += \
           mainwindow.ui \
    mainwindowoptions.ui

RESOURCES += \
    mainwindow.qrc
