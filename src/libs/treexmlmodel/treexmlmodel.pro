include(../libs.pri)
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
QT += xml
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

