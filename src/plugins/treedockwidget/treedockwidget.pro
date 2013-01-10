LIBS += -lfiltertreewidget -lplugin

include(../plugins.pri)

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
