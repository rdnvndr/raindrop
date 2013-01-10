TEMPLATE = subdirs
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
HEADERS += \
    imainwindow.h \
    itreedockwidget.h
