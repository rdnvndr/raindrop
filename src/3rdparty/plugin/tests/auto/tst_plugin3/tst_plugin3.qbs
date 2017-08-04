import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_plugin3"
    type: "dynamiclibrary"

    productname: "TST_PLUGIN3"
    version:     "1.0.0"
    description: "TST_PLUGIN3"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["TST_PLUGIN3_LIBRARY"])
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
        fileTagsFilter: [
            "dynamiclibrary",
            "dynamiclibrary_import"]
        qbs.install: true
        qbs.installDir: "tests/auto/plugin/plugins"
    }
}
