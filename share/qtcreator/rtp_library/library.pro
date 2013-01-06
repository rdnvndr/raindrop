include(../libs.pri)
DEFINES += %LibName:u%_LIBRARY
QT += widgets
# Input
HEADERS += %LibName:l%.%CppHeaderSuffix% %LibName:l%global.%CppHeaderSuffix:l%
SOURCES += %LibName:l%.%CppSourceSuffix%