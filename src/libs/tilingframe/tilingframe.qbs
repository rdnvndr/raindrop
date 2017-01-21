import "../main.qbs" as LibProduct

LibProduct {
    name: "tilingframe"

    productname: "TilingFrame Library"
    version:     "1.0.0"
    description: "TilingFrame Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["TILINGFRAME_LIBRARY"])
    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "tilingframe.h",
        "tilingframeglobal.h",
        "tilingwidget.h",
        "version.h",
        "tilingframe.cpp",
        "tilingwidget.cpp"
    ]
}