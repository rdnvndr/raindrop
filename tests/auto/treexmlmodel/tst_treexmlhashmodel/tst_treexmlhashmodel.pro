include(../../auto.pri)
QT += xml
LIBS += -ltreexmlmodel
INCLUDEPATH += $$PWD/../
SOURCES  += tst_treexmlhashmodel.cpp  ../modeltest/dynamictreemodel.cpp ../modeltest/modeltest.cpp
HEADERS  += ../modeltest/dynamictreemodel.h ../modeltest/modeltest.h

RESOURCES += \
    tst_treexmlhashmodel.qrc
