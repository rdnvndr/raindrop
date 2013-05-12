######################################################################
# Automatically generated by qmake (2.01a) ?? 25. ??? 15:05:28 2011
######################################################################
DEPENDPATH += .
INCLUDEPATH += .

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
QT += xml
LIBS += -lplugin -ltreedockwidget -lmdiextarea -ltreexmlmodel -ltreecombobox -lpushbuttonimage
include(../plugins.pri)
# Input
HEADERS += modelerideplug.h \
    propclass.h \
    dbxmlstruct.h \
    classtreeview.h \
    treefilterproxymodel.h \
    attrwidget.h \
    classwidget.h \
    compositionwidget.h \
    propcomposition.h \
    compositionpropwidget.h \
    xmldelegate.h \
    filterwidget.h \
    propfilter.h \
    filterpropwidget.h \
    conditionproxymodel.h
SOURCES += modelerideplug.cpp \
    propclass.cpp \
    classtreeview.cpp \
    treefilterproxymodel.cpp \
    attrwidget.cpp \
    classwidget.cpp \
    compositionwidget.cpp \
    propcomposition.cpp \
    compositionpropwidget.cpp \
    xmldelegate.cpp \
    filterwidget.cpp \
    propfilter.cpp \
    filterpropwidget.cpp \
    conditionproxymodel.cpp
RESOURCES += modeleride.qrc

FORMS += \
    propclass.ui \
    classtreeview.ui \
    attrwidget.ui \
    classwidget.ui \
    compositionwidget.ui \
    propcomposition.ui \
    compositionpropwidget.ui \
    filterwidget.ui \
    propfilter.ui \
    filterpropwidget.ui
