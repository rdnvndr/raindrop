import "../main.qbs" as AppProduct

AppProduct {
    name: "raindrop"

    productname: "Raindrop"
    version:     "1.0.0"
    description: "Main program"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin"}
    cpp.defines: base.concat([
        "PLUGIN_DIR=\"" + product.rPluginPath + "\"",
    ])

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
}
