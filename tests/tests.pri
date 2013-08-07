include(../main.pri)
INCLUDEPATH += $$PWD/../src/libs $$PWD/../src/shared $$PWD/../src/plugins

QMAKE_RPATHDIR += $$LIBRARY_PATH
unix {
  QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,$${QMAKE_RPATHDIR}\'
}

LIBS +=-L$$LIBRARY_PATH
LIBS +=-L$$PLUGINS_PATH

TEMPLATE = app
# DESTDIR = $$APP_PATH

# prefix test binary with tst_
!contains(TARGET, ^tst_.*):TARGET = $$join(TARGET,,"tst_")
win32 {
    lib = $$APP_PATH;$$APP_PATH/plugins
    lib ~= s,/,\\,g
    # the below gets added to later by testcase.prf    
    check.commands = cd . & set PATH=$$lib;%PATH%& cmd /c
}
