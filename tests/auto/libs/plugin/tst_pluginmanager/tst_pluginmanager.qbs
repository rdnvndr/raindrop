import "../main.qbs" as PluginProduct

PluginProduct {
    name: "tst_pluginmanager"

    productname: ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

//    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets", "testlib"] }
    Depends { name: "plugin" }

    files: [
        "pluginloadlog.h",
        "pluginloadlog.cpp",
        "tst_pluginmanager.cpp"
    ]
}
