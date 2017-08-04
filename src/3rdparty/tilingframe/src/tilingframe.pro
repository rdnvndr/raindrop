VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "TilingFrame Library"
QMAKE_TARGET_DESCRIPTION = "TilingFrame Library"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

include(../libs.pri)

DEFINES += TILINGFRAME_LIBRARY
QT += widgets
# Input
HEADERS   = *.h
SOURCES   = *.cpp
