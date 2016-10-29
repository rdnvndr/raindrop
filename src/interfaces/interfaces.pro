TEMPLATE = subdirs
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}
HEADERS += imainwindow.h
HEADERS += itreedockwidget.h
HEADERS += iundogroup.h
HEADERS += iclipboardstack.h
HEADERS += iclipboarditem.h

HEADERS += idatabaseitem.h
HEADERS += idatabasemodel.h
HEADERS += idatabaseattr.h
HEADERS += idatabaseclass.h
HEADERS += idatabasefilter.h
HEADERS += idatabasecomposition.h
HEADERS += idatabaserole.h
HEADERS += idatabaseexpression.h

HEADERS += idatabasemodelbuilder.h
