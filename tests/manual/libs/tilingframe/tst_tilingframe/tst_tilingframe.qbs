import "../main.qbs" as Product

Product {
    name: "tst_tilingframe_mnl"

    product:     ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.dynamicLibraries: base.concat(["tilingframe"])
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }

    files: [
        "mainwindow.h",
        "tilingbutton.h",
        "main.cpp",
        "mainwindow.cpp",
        "tilingbutton.cpp",
        "mainwindow.ui",
        "rsrc.qrc"
    ]
}
