import "../main.qbs" as Product

Product {
    name: "plugin"

    product:     "Plugin Library"
    version:     "1.0.0"
    description: "Plugin Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["PLUGIN_LIBRARY"])
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
