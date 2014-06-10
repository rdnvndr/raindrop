include(../auto.pri)
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

!greaterThan(QT_MAJOR_VERSION, 4) {
  load(qttest_p4)
}

SOURCES         += tst_modeltest.cpp modeltest.cpp dynamictreemodel.cpp
HEADERS         += modeltest.h dynamictreemodel.h



