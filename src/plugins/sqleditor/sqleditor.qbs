import "../main.qbs" as Product

Product {
    name: "sqleditor"

    product:     "SQL редактор"
    version:     "1.0.0"
    description: "SqlEditor Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["SQLEDITOR_LIBRARY"])
    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets", "sql"] }

    files: [
        "querysqlwidget.h",
        "sqleditor.h",
        "sqleditorglobal.h",
        "sqlhighlighter.h",
        "textclipboarditem.h",
        "undotextdocument.h",
        "version.h",
        "querysqlwidget.cpp",
        "sqleditor.cpp",
        "sqlhighlighter.cpp",
        "textclipboarditem.cpp",
        "undotextdocument.cpp",
        "querysqlwidget.ui",
        "sqleditor.qrc"
    ]
}
