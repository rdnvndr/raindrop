import "../main.qbs" as PluginProduct

PluginProduct {
    name: "dbconnect"

    productname: "Подключение к БД"
    version:     "1.0.0"
    description: "DbConnect Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["DBCONNECT_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets", "sql", "core"] }
    Depends { name: "plugin" }

    files: [
        "dbconnect.h",
        "dbconnectglobal.h",
        "dialogconnect.h",
        "threadconnect.cpp",
        "threadconnect.h",
        "version.h",
        "dbconnect.cpp",
        "dialogconnect.cpp",
        "dialogconnect.ui",
        "dbconnect.qrc",
    ]
}
