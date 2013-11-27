include(../main.pri)
INCLUDEPATH += $$PWD/libs $$PWD/shared $$PWD/plugins $$PWD/interfaces


QMAKE_RPATHDIR += .
unix:!macx {
  QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,$${QMAKE_RPATHDIR}\'
}

macx {
  CONFIG-=app_bundle
}

LIBS +=-L$$LIBRARY_PATH
LIBS +=-L$$PLUGINS_PATH
