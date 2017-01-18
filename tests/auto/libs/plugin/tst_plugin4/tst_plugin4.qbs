import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_plugin4"
    type: "dynamiclibrary"

    productname: "TST_PLUGIN4"
    version:     "1.0.0"
    description: "TST_PLUGIN4"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["TST_PLUGIN4_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }

    files: [
        "itst_plugin4.h",
        "tst_plugin4.h",
        "tst_plugin4global.h",
        "tst_plugin4.cpp",
        "tst_plugin4.qrc"
    ]
}
