include(../../subdir.pri)
QT += xml
LIBS += -ltreexmlmodel
SOURCES  += tst_tablexmlproxymodel.cpp  $$SHAREDIR/modeltest/dynamictreemodel.cpp $$SHAREDIR/modeltest/modeltest.cpp
HEADERS  += $$SHAREDIR/modeltest/dynamictreemodel.h $$SHAREDIR/modeltest/modeltest.h 

RESOURCES += \
    tst_tablexmlproxymodel.qrc
