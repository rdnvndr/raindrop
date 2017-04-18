import "../main.qbs" as PluginProduct

PluginProduct {
    name: "pgdatabasemodel"

    productname: "Реализация модели базы данных PostgreSql"
    version:     "1.0.0"
    description: "Реализация модели базы данных PostgreSql"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["PGDATABASEMODEL_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets", "sql"] }
    Depends { name: "plugin" }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
}
