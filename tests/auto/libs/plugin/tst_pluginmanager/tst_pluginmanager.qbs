import "../main.qbs" as Product

Product {
    name: "tst_pluginmanager"

    product:     ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "pluginloadlog.h",
        "pluginloadlog.cpp",
        "tst_pluginmanager.cpp"
    ]
}
