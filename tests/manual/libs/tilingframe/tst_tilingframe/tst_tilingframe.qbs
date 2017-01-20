import "../main.qbs" as TilingFrameProduct

TilingFrameProduct {
    name: "tst_tilingframe_mnl"

    productname: ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }
    Depends { name: "tilingframe" }

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
