import "../main.qbs" as LibProduct

LibProduct {
    name: "dockwidget"

    productname: "DockWidget Library"
    version:     "1.0.0"
    description: "DockWidget Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["DOCKWIDGET_LIBRARY"])
    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
}
