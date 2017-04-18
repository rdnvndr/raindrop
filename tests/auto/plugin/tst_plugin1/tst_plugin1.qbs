import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_plugin1"
    type: "dynamiclibrary"

    productname: "TST_PLUGIN1"
    version:     "1.0.0"
    description: "TST_PLUGIN1"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["TST_PLUGIN1_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
    Group {
        fileTagsFilter: ["dynamiclibrary"]
        qbs.install: true
        qbs.installDir: "tests/auto/tst_plugin/plugins"
    }
}
