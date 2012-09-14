LIBS += -lcommon

include(../libs.pri)

TARGET = extensionsystem
TEMPLATE = lib

DEFINES += EXTENSIONSYSTEM_LIBRARY

SOURCES += \
        pluginmanager.cpp \
    iplugin.cpp \
    plugincollection.cpp \
    objectcollection.cpp \
    iguiplugin.cpp \
    pluginviewdlg.cpp \
    pluginmodel.cpp \
    pluginmodelitem.cpp

HEADERS +=\
        pluginmanager.h \
    iplugin.h \
    plugincollection.h \
    objectcollection.h \
    iguiplugin.h \
    pluginviewdlg.h \
    extensionsystemglobal.h \
    pluginmodel.h \
    pluginmodelitem.h

FORMS += \
    pluginviewdlg.ui

RESOURCES += \
    extensionsystem.qrc
