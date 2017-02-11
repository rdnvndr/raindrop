import "../main.qbs" as PluginProduct

PluginProduct {
    name: "databasemodel"

    productname: "Модель базы данных"
    version:     "1.0.0"
    description: "Модель базы данных"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["DATABASEMODEL_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets", "sql"] }
    Depends { name: "plugin" }

    files: [
        "databasemodel.h",
        "databasemodelglobal.h",
        "version.h",
        "databasemodel.cpp",
        "databasemodel.qrc"
    ]
}
