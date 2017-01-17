import "../main.qbs" as Product

Product {
    name: "raindrop"

    product:     "Raindrop"
    version:     "1.0.0"
    description: "Main program"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    files: [
        "main.cpp",
        "raindrop.qrc",
        "version.h"
    ]
}
