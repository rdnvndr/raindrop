import "../main.qbs" as PluginProduct

PluginProduct {
    name: "clipboardstack"

    productname: "Clipboard Stack"
    version:     "1.0.0"
    description: "Clipboard Stack Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["CLIPBOARDSTACK_LIBRARY"])
//    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }

    files: [
        "clipboardstack.h",
        "clipboardstackglobal.h",
        "version.h",
        "clipboardstack.cpp",
        "clipboardstack.qrc"
    ]
}
