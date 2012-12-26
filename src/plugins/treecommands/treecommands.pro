DEPENDPATH += .
INCLUDEPATH += .

QT += widgets
LIBS += -lfiltertreewidget -lplugin -ltreedockwidget
include(../plugins.pri)
# Input
HEADERS += treecommandsplug.h
SOURCES += treecommandsplug.cpp

FORMS +=

RESOURCES += \
    treecommands.qrc
