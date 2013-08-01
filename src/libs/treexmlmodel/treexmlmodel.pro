include(../libs.pri)
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
QT += xml
DEFINES += XMLMODEL_LIBRARY
HEADERS += \
    tagxmlitem.h \
    tablexmlproxymodel.h \
    treexmlmodelglobal.h \
    modifyproxymodel.h \
    treexmlhashmodel.h

SOURCES += \ 
    tagxmlitem.cpp \
    tablexmlproxymodel.cpp \
    modifyproxymodel.cpp \
    treexmlhashmodel.cpp

