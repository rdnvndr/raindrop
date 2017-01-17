import "../main.qbs" as LibProduct

LibProduct {
    name: "mdiextarea"

    product:     "MdiExtArea Library"
    version:     "1.0.0"
    description: "MdiExtArea Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["MDIEXTAREA_LIBRARY"])
    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "mdiextarea.h",
        "mdiextareaglobal.h",
        "version.h",
        "mdiextarea.cpp"
    ]
}
