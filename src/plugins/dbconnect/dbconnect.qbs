import "../main.qbs" as PluginProduct

PluginProduct {
    name: "dbconnect"

    productname: "Подключение к БД"
    version:     "1.0.0"
    description: "DbConnect Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["DBCONNECT_LIBRARY"])
//    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets", "sql"] }
    Depends { name: "plugin" }

    files: [
        "dbconnect.h",
        "dbconnectglobal.h",
        "dialogconnect.h",
        "version.h",
        "dbconnect.cpp",
        "dialogconnect.cpp",
        "dialogconnect.ui",
        "dbconnect.qrc"
    ]
}
