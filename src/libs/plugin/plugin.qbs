import "../main.qbs" as LibProduct

LibProduct {
    name: "plugin"

    productname: "Plugin Library"
    version:     "1.0.0"
    description: "Plugin Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat([
        "PLUGIN_LIBRARY",
        "PLUGIN_DIR=\"" + product.rPluginPath + "\"",
    ])
    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "iplugin.h",
        "pluginglobal.h",
        "pluginmanager.h",
        "version.h",
        "iplugin.cpp",
        "pluginmanager.cpp"
    ]
}
