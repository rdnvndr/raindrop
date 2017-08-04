import "../main.qbs" as PluginProduct

PluginProduct {
    name: "mainwindow"

    productname: "Главное окно"
    version:     "1.0.0"
    description: "MainWindow Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["MAINWINDOW_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }
    Depends { name: "mdiextarea" }
    Depends { name: "guiextension" }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
}
