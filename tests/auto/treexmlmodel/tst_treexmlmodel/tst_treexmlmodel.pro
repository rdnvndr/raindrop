include(../../auto.pri)
QT += xml
LIBS += -ltreexmlmodel
SOURCES  += tst_treexmlmodel.cpp  ../modeltest/dynamictreemodel.cpp ../modeltest/modeltest.cpp
HEADERS  += ../modeltest/dynamictreemodel.h ../modeltest/modeltest.h
INCLUDEPATH += $$PWD/../
RESOURCES += \
    tst_treexmlmodel.qrc
