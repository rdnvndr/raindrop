import "../main.qbs" as Product

Product {
    name: "example"

    product:     ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    files: [
        "exampleglobal.h",
        "exampleplug.h",
        "version.h",
        "exampleplug.cpp",
        "example.qrc"
    ]
}
