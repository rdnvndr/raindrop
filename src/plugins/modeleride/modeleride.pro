######################################################################
# Automatically generated by qmake (2.01a) ?? 25. ??? 15:05:28 2011
######################################################################
VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Редактор модели данных"
QMAKE_TARGET_DESCRIPTION = "ModelerIDE Plugin"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

DEPENDPATH += .
INCLUDEPATH += .

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
QT += xml

LIBS += -lplugin -lmdiextarea -ltreexmlmodel -ltreecombobox -lpushbuttonimage -ldockwidget
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
    xmldelegate.h \
    filterwidget.h \
    propfilter.h \
    filterpropwidget.h \
    conditionproxymodel.h \
    conditiondelegate.h \
    msrunitwidget.h \
    msrentitywidget.h \
    propentity.h \
    regexpvalidator.h \
    propentitygroup.h \
    unitdelegate.h \
    lovwidget.h \
    proplov.h \
    lovvaluewidget.h \
    lovdelegate.h \
    proprefgroup.h \
    propref.h \
    refwidget.h \
    refitemwidget.h \
    refitemdelegate.h \
    refproxymodel.h \
    classmodel.h \
    attrgroupproxymodel.h
SOURCES += modelerideplug.cpp \
    propclass.cpp \
    classtreeview.cpp \
    treefilterproxymodel.cpp \
    attrwidget.cpp \
    classwidget.cpp \
    compositionwidget.cpp \
    xmldelegate.cpp \
    filterwidget.cpp \
    propfilter.cpp \
    filterpropwidget.cpp \
    conditionproxymodel.cpp \
    conditiondelegate.cpp \
    msrunitwidget.cpp \
    msrentitywidget.cpp \
    propentity.cpp \
    regexpvalidator.cpp \
    propentitygroup.cpp \
    unitdelegate.cpp \
    lovwidget.cpp \
    proplov.cpp \
    lovvaluewidget.cpp \
    lovdelegate.cpp \
    proprefgroup.cpp \
    propref.cpp \
    refwidget.cpp \
    refitemwidget.cpp \
    refitemdelegate.cpp \
    refproxymodel.cpp \
    classmodel.cpp \
    attrgroupproxymodel.cpp
RESOURCES += modeleride.qrc

FORMS += \
    propclass.ui \
    classtreeview.ui \
    attrwidget.ui \
    classwidget.ui \
    compositionwidget.ui \
    filterwidget.ui \
    propfilter.ui \
    filterpropwidget.ui \
    msrunitwidget.ui \
    msrentitywidget.ui \
    propentity.ui \
    propentitygroup.ui \
    lovwidget.ui \
    proplov.ui \
    lovvaluewidget.ui \
    proprefgroup.ui \
    propref.ui \
    refwidget.ui \
    refitemwidget.ui
