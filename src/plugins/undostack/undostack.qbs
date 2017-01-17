import "../main.qbs" as Product

Product {
    name: "undostack"

    product:     "UndoStack"
    version:     "1.0.0"
    description: "Undo Stack Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["UNDOSTACK_LIBRARY"])
    cpp.dynamicLibraries: base.concat(["plugin"])
    cpp.includePaths:     base.concat([ '.'])

    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "undostack.h",
        "undostackglobal.h",
        "version.h",
        "undostack.cpp",
        "undostack.qrc"
    ]
}
