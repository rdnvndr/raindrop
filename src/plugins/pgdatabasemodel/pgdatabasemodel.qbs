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
        "pgdatabaseattribute.cpp",
        "pgdatabaseattribute.h",
        "pgdatabaseattrs.cpp",
        "pgdatabaseattrs.h",
        "pgdatabaseclass.h",
        "pgdatabaseclasses.cpp",
        "pgdatabaseclasses.h",
        "pgdatabasecomposition.h",
        "pgdatabasecomps.cpp",
        "pgdatabasecomps.h",
        "pgdatabaseexpression.h",
        "pgdatabasefilter.h",
        "pgdatabasefilters.cpp",
        "pgdatabasefilters.h",
        "pgdatabasemodel.cpp",
        "pgdatabasemodel.h",
        "pgdatabasemodelbuilder.cpp",
        "pgdatabasemodelbuilder.h",
        "pgdatabasemodelglobal.h",
        "pgdatabaseclass.cpp",
        "pgdatabasecomposition.cpp",
        "pgdatabaseexpression.cpp",
        "pgdatabasefilter.cpp",
        "version.h",
        "pgdatabasemodel.qrc",
    ]
}
