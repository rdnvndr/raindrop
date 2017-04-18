import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_plugin2"
    type: "dynamiclibrary"

    productname: "TST_PLUGIN2"
    version:     "1.0.0"
    description: "TST_PLUGIN2"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["TST_PLUGIN2_LIBRARY"])
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
        qbs.installDir: "tests/auto/tst_plugin/plugins"
    }
}
