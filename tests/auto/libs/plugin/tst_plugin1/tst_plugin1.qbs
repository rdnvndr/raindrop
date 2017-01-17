import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_plugin1"
    type: "dynamiclibrary"

    productname: "TST_PLUGIN1"
    version:     "1.0.0"
    description: "TST_PLUGIN1"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["TST_PLUGIN1_LIBRARY"])
    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "imainwindow.h",
        "itst_plugin1.h",
        "tst_plugin1.h",
        "tst_plugin1global.h",
        "tst_plugin1.cpp",
        "tst_plugin1.qrc"
    ]
}
