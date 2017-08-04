import "../main.qbs" as PluginProduct

PluginProduct {
    name: "modeleride"

    productname: "Редактор модели данных"
    version:     "1.0.0"
    description: "ModelerIDE Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets", "xml"] }
    Depends { name: "plugin" }
    Depends { name: "mdiextarea" }
    Depends { name: "treexmlmodel" }
    Depends { name: "guiextension" }
    Depends { name: "metadatamodel" }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
    Group {
        name: "Example"
        files: "data/system.xml"
        qbs.install: true
        qbs.installDir: product.docPath + "/modeleride/"
    }
}
