import "../main.qbs" as PluginProduct

PluginProduct {
    name: "dbmodelexample"

    productname: "DbModelExample"
    version:     "1.0.0"
    description: "Пример работы с DatabaseModel"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["DBMODELEXAMPLE_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }

    files: [
        "dbmodelexample.h",
        "dbmodelexampleglobal.h",
        "dbmodelexample.cpp",
        "version.h",
        "dbmodelexample.qrc"
    ]
}