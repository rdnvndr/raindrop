import "../main.qbs" as Product

Product {
    name: "tst_tablexmlproxymodel"

    product:     ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.dynamicLibraries: base.concat(["treexmlmodel"])
    Depends { name: "Qt"; submodules: ["xml"] }

    files: [
        "tst_tablexmlproxymodel.cpp",
        "tst_tablexmlproxymodel.qrc"
    ]
}
