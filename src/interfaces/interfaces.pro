TEMPLATE = subdirs
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
HEADERS += imainwindow.h
HEADERS += itreedockwidget.h
HEADERS += iundogroup.h
HEADERS += iclipboardstack.h
HEADERS += iclipboarditem.h
HEADERS += iclipboardelement.h
HEADERS += idatabaseconnect.h