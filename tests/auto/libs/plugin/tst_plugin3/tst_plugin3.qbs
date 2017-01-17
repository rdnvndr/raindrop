import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_plugin3"
    type: "dynamiclibrary"

    product:     "TST_PLUGIN3"
    version:     "1.0.0"
    description: "TST_PLUGIN3"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["TST_PLUGIN3_LIBRARY"])
    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "itst_plugin3.h",
        "tst_plugin3.h",
        "tst_plugin3global.h",
        "tst_plugin3.cpp",
        "tst_plugin3.qrc"
    ]
}
