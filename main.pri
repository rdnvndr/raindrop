# Save Version
win32 {
   HEADERS += version.h
   version.commands = $$replace(PWD,"/","\\")\\scripts\\version.bat
}
unix {
   HEADERS += version.h
   version.commands = $$PWD/scripts/version.sh
}
QMAKE_EXTRA_TARGETS += version
PRE_TARGETDEPS = version


# Shadow Build
contains($${PWD}, $${OUT_PWD}) {
  BUILD_PATH = $$PWD
}

isEmpty(BUILD_PATH) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$${PWD},,
    BUILD_PATH = $$OUT_PWD
    BUILD_PATH ~= s,$${sub_dir}$,,
}

APP_PATH      = $$BUILD_PATH/bin
BIN_PATH      = $$APP_PATH
LIBRARY_PATH  = $$APP_PATH
PLUGINS_PATH  = $$APP_PATH/plugins

listlib = $$find(LIBS, ^-l)
for(onelib, listlib) {
    onelib ~= s,^-l,,

    exists($$BUILD_PATH/src/shared/$$onelib){
        INCLUDEPATH += $$BUILD_PATH/src/shared/$$onelib $$PWD/src/shared/$$onelib
    }

    exists($$BUILD_PATH/src/libs/$$onelib){
        INCLUDEPATH += $$BUILD_PATH/src/libs/$$onelib $$PWD/src/libs/$$onelib
    }

    exists($$BUILD_PATH/src/plugins/$$onelib){
        INCLUDEPATH += $$BUILD_PATH/src/plugins/$$onelib $$PWD/src/plugins/$$onelib
    }
}
