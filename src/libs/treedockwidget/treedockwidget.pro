LIBS += -lfiltertreewidget

include(../libs.pri)
# Input

DEFINES += TREEDOCKWIDGET_LIBRARY


HEADERS += treedockwidget.h \
           treefilterwidget.h \
    treedockwidgetglobal.h
FORMS += treefilterwidget.ui
SOURCES += treedockwidget.cpp treefilterwidget.cpp
RESOURCES += \
    treedockwidget.qrc
