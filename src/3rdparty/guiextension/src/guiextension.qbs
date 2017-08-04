import "./main.qbs" as LibProduct

LibProduct {
    name: "guiextension"

    productname: "GuiExtension Library"
    version:     "1.0.0"
    description: "GuiExtension Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["GUIEXTENSION_LIBRARY"])
    Depends { name: "Qt"; submodules: ["widgets", "core"] }

    files: [
        "*.h",
        "*.cpp",
        "*.ui",
        "*.qrc"
    ]
}
