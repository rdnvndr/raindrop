import "../main.qbs" as LibProduct

LibProduct {
    name: "treexmlmodel"

    productname: "TreeXmlModel Library"
    version:     "1.0.0"
    description: "TreeXmlModel Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["XMLMODEL_LIBRARY"])
    Depends { name: "Qt"; submodules: ["widgets", "xml"] }

    files: [
        "mimedataindex.h",
        "modifyproxymodel.h",
        "setdatacommand.h",
        "tablexmlproxymodel.h",
        "tagxmlitem.h",
        "treexmlhashmodel.h",
        "treexmlmodel.h",
        "treexmlmodelglobal.h",
        "version.h",
        "mimedataindex.cpp",
        "modifyproxymodel.cpp",
        "setdatacommand.cpp",
        "tablexmlproxymodel.cpp",
        "tagxmlitem.cpp",
        "treexmlhashmodel.cpp",
        "treexmlmodel.cpp"
    ]
}

