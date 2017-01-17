import "../main.qbs" as Product

Product {
    name: "tst_plugin4"
    type: "dynamiclibrary"

    product:     "TST_PLUGIN4"
    version:     "1.0.0"
    description: "TST_PLUGIN4"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["TST_PLUGIN4_LIBRARY"])
    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "itst_plugin4.h",
        "tst_plugin4.h",
        "tst_plugin4global.h",
        "tst_plugin4.cpp",
        "tst_plugin4.qrc"
    ]
}
