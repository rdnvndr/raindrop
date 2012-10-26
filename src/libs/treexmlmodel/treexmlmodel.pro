include(../libs.pri)
QT += xml
DEFINES += XMLMODEL_LIBRARY
HEADERS += \
    treexmlmodel.h \
    tagxmlitem.h \
    xmlmodeldll.h \
    tablexmlproxymodel.h \
    treexmlmodelglobal.h

SOURCES += \ 
    treexmlmodel.cpp \
    tagxmlitem.cpp \
    tablexmlproxymodel.cpp

