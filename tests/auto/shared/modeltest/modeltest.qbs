import "../main.qbs" as SharedProduct

SharedProduct {
    name: "modeltest"

    productname: ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "dynamictreemodel.h",
        "modeltest.h",
        "dynamictreemodel.cpp",
        "modeltest.cpp",
        "tst_modeltest.cpp"
    ]
}
