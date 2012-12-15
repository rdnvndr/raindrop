include(../main.pri)
INCLUDEPATH += $$PWD/libs $$PWD/shared $$PWD/plugins


QMAKE_RPATHDIR += .
unix {
  QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,$${QMAKE_RPATHDIR}\'
}

LIBS +=-L$$LIBRARY_PATH
LIBS +=-L$$PLUGINS_PATH