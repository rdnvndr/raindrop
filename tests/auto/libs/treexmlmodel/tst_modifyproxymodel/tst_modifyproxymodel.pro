include(../../subdir.pri)
QT += xml
LIBS += -ltreexmlmodel
SOURCES  += tst_modifyproxymodel.cpp $$SHAREDIR/modeltest/dynamictreemodel.cpp $$SHAREDIR/modeltest/modeltest.cpp
HEADERS  += $$SHAREDIR/modeltest/dynamictreemodel.h $$SHAREDIR/modeltest/modeltest.h 

RESOURCES += \
    tst_modifyproxymodel.qrc
