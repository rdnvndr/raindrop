import "../main.qbs" as Product

Product {
    name: "metadatamodel"

    product:     "MetadataModel Library"
    version:     "1.0.0"
    description: "MetadataModel Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["METADATAMODEL_LIBRARY"])
    cpp.dynamicLibraries: base.concat(["treexmlmodel"])

    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "classmodelxml.h",
        "dbxmlstruct.h",
        "metadatamodelglobal.h",
        "version.h",
        "classmodelxml.cpp",
        "metadatamodel.qrc"
    ]
}
