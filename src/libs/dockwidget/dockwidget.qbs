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
        "dockwidget.h",
        "dockwidgetglobal.h",
        "version.h",
        "dockwidget.cpp"
    ]
}
