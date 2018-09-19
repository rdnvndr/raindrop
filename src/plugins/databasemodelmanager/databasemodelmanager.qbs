import "../main.qbs" as PluginProduct

PluginProduct {
    name: "databasemodelmanager"

    productname: "Менеджер моделей баз данных"
    version:     "1.0.0"
    description: "Менеджер моделей баз данных"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["DATABASEMODELMANAGER_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets", "sql"] }
    Depends { name: "plugin" }
    Depends { name: "sqlextension" }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
}
