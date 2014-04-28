include(../src.pri)
CONFIG += plugin
TEMPLATE = lib
DESTDIR = $$PLUGINS_PATH
win32 {
    TARGET_EXT = .dll
}
