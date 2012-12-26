include(../libs.pri)
QT += xml widgets
DEFINES += XMLMODEL_LIBRARY
HEADERS += \
    treexmlmodel.h \
    tagxmlitem.h \
    tablexmlproxymodel.h \
    treexmlmodelglobal.h

SOURCES += \ 
    treexmlmodel.cpp \
    tagxmlitem.cpp \
    tablexmlproxymodel.cpp

