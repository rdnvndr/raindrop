import "../main.qbs" as Product

Product {
    name: "dbconnect"

    product:     ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    files: [
        "dbconnect.h",
        "dbconnectglobal.h",
        "dialogconnect.h",
        "version.h",
        "dbconnect.cpp",
        "dialogconnect.cpp",
        "dialogconnect.ui",
        "dbconnect.qrc"
    ]
}
