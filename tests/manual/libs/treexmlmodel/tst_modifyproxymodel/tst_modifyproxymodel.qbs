import "../main.qbs" as TreeXmlModelProduct

TreeXmlModelProduct {
    name: "tst_modifyproxymodel_mnl"

    productname: ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "treexmlmodel" }

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
