import "../main.qbs" as SqlExtensionProduct

SqlExtensionProduct {
    name: "tst_sqlextension_mnl"

    productname: ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "sql"] }
    Depends { name: "sqlextension" }

    files: [
        "mainwindow.h",
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.ui"
    ]

    Group {
        name: "Runtime resources"
        files: "*.db"
        qbs.install: true
        qbs.installDir: "tests/manual/" + "/"
    }
}
