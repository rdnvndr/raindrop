LIBS += -lfiltertreewidget

include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .

# Input

QT += widgets

DEFINES += TREEDOCKWIDGET_LIBRARY


HEADERS += treedockwidget.h \
           treefilterwidget.h \
    treedockwidgetglobal.h
FORMS += treefilterwidget.ui
SOURCES += treedockwidget.cpp treefilterwidget.cpp
RESOURCES += \
    treedockwidget.qrc
