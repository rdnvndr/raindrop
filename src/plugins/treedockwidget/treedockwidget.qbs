import "../main.qbs" as Product

Product {
    name: "treedockwidget"

    product:     "Дерево команд"
    version:     "1.0.0"
    description: "TreeDockWidget Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["SQLEDITOR_LIBRARY"])
    cpp.dynamicLibraries: base.concat(["plugin", "filtertreewidget"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets", "sql"] }

    files: [
        "treedockwidget.h",
        "treedockwidgetglobal.h",
        "treefilterwidget.h",
        "version.h",
        "treedockwidget.cpp",
        "treefilterwidget.cpp",
        "treefilterwidget.ui",
        "treedockwidget.qrc"
    ]
}
