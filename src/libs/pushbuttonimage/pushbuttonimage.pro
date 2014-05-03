VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "PushButtonImage Library"
QMAKE_TARGET_DESCRIPTION = "PushButtonImage Library"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

include(../libs.pri)

DEFINES += PUSHBUTTONIMAGE_LIBRARY
QT += widgets
# Input
HEADERS += pushbuttonimage.h pushbuttonimageglobal.h
SOURCES += pushbuttonimage.cpp
