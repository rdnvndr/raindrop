import "../main.qbs" as TreeXmlModelProduct

TreeXmlModelProduct {
    name: "tst_modifyproxymodel_mnl"

    product:     ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.dynamicLibraries: base.concat(["treexmlmodel"])
    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "mainwindow.h",
        "treeitem.h",
        "treemodel.h",
        "main.cpp",
        "mainwindow.cpp",
        "treeitem.cpp",
        "treemodel.cpp",
        "mainwindow.ui",
        "modifyproxymodel.qrc"
    ]
}
