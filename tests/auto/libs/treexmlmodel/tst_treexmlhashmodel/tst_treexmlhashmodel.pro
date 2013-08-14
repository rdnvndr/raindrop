include(../../subdir.pri)
QT += xml
LIBS += -ltreexmlmodel

SOURCES  += tst_treexmlhashmodel.cpp  $$SHAREDIR/modeltest/dynamictreemodel.cpp $$SHAREDIR/modeltest/modeltest.cpp
HEADERS  += $$SHAREDIR/modeltest/dynamictreemodel.h $$SHAREDIR/modeltest/modeltest.h 

RESOURCES += \
    tst_treexmlhashmodel.qrc
