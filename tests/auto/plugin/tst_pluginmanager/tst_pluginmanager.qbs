import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_pluginmanager"

    productname: ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets", "testlib"] }
    Depends { name: "plugin" }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
    Group {
        fileTagsFilter: ["application"]
        qbs.install: true
        qbs.installDir: "tests/auto/tst_plugin/"
    }
}
