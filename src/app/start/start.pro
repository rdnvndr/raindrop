LIBS += -lextensionsystem
include(../app.pri)

TARGET = start

SOURCES += main.cpp \
    extendedapplication.cpp

HEADERS += \
    extendedapplication.h
