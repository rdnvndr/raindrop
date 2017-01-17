import "../main.qbs" as Product

Product {
    name: "plugin"

    product:     ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    files: [
        "iplugin.h",
        "pluginglobal.h",
        "pluginmanager.h",
        "version.h",
        "iplugin.cpp",
        "pluginmanager.cpp"
    ]
}
