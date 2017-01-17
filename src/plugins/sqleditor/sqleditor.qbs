import "../main.qbs" as Product

Product {
    name: "sqleditor"

    product:     ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

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
