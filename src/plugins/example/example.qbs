import "../main.qbs" as Product

Product {
    name: "example"

    product:     "Пример"
    version:     "1.0.0"
    description: "Example Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["EXAMPLE_LIBRARY"])
    cpp.dynamicLibraries: base.concat(["plugin", "mdiextarea"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "exampleglobal.h",
        "exampleplug.h",
        "version.h",
        "exampleplug.cpp",
        "example.qrc"
    ]
}
