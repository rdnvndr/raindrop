include(../libs.pri)
QT += xml
DEFINES += XMLMODEL_LIBRARY
HEADERS += \
    treexmlmodel.h \
    tagxmlitem.h \
    xmlmodeldll.h \
    tablexmlproxymodel.h \
    xmldelegate.h \
    treexmlmodelglobal.h

SOURCES += \ 
    treexmlmodel.cpp \
    tagxmlitem.cpp \
    tablexmlproxymodel.cpp \
    xmldelegate.cpp

