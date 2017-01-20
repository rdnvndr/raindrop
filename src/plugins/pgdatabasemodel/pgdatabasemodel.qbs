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

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }

    files: [
        "pgdatabaseattr.h",
        "pgdatabaseclass.h",
        "pgdatabasecomposition.h",
        "pgdatabaseexpression.h",
        "pgdatabasefilter.h",
        "pgdatabasemodel.h",
        "pgdatabasemodelglobal.h",
        "pgdatabaseattr.cpp",
        "pgdatabaseclass.cpp",
        "pgdatabasecomposition.cpp ",
        "pgdatabaseexpression.cpp",
        "pgdatabasefilter.cpp",
        "pgdatabasemodel.cpp",
        "version.h",
        "pgdatabasemodel.qrc"
    ]
}
