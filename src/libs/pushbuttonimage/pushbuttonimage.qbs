import "../main.qbs" as LibProduct

LibProduct {
    name: "pushbuttonimage"

    productname: "PushButtonImage Library"
    version:     "1.0.0"
    description: "PushButtonImage Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["PUSHBUTTONIMAGE_LIBRARY"])
    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
}
