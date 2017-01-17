import "../main.qbs" as TreeXmlModelProduct

TreeXmlModelProduct {
    name: "tst_tablexmlproxymodel"

    productname: ""
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
