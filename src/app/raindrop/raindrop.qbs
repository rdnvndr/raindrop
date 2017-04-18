import "../main.qbs" as AppProduct

AppProduct {
    name: "raindrop"

    productname: "Raindrop"
    version:     "1.0.0"
    description: "Main program"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin"}

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
}
