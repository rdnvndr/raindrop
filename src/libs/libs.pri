include(../src.pri)
TEMPLATE = lib
DESTDIR = $$LIBRARY_PATH
win32 {
    TARGET_EXT = .dll
}

