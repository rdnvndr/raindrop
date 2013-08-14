include(../../libs.pri)
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
LIBS += -ltreexmlmodel
CONFIG  += qt
FORMS       = mainwindow.ui
HEADERS     = mainwindow.h \
              treeitem.h \
              treemodel.h
RESOURCES   = \
    modifyproxymodel.qrc
SOURCES     = mainwindow.cpp \
              treeitem.cpp \
              treemodel.cpp \
              main.cpp



