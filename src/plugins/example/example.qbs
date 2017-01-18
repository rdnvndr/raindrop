import "../main.qbs" as PluginProduct

PluginProduct {
    name: "example"

    productname: "Пример"
    version:     "1.0.0"
    description: "Example Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["EXAMPLE_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }
    Depends { name: "mdiextarea" }

    files: [
        "exampleglobal.h",
        "exampleplug.h",
        "version.h",
        "exampleplug.cpp",
        "example.qrc"
    ]
}
