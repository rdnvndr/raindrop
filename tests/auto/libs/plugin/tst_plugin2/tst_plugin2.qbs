import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_plugin2"
    type: "dynamiclibrary"

    productname: "TST_PLUGIN2"
    version:     "1.0.0"
    description: "TST_PLUGIN2"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["TST_PLUGIN2_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }

    files: [
        "itst_plugin2.h",
        "tst_plugin2.h",
        "tst_plugin2global.h",
        "tst_plugin2.cpp",
        "tst_plugin2.qrc"
    ]
}