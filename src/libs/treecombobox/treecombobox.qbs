import "../main.qbs" as Product

Product {
    name: "treecombobox"

    product:     "TreeComboBox Library"
    version:     "1.0.0"
    description: "TreeComboBox Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["TREECOMBOBOX_LIBRARY"])
    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "treecombobox.h",
        "treecomboboxglobal.h",
        "version.h",
        "treecombobox.cpp"
    ]
}
