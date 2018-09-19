import "../main.qbs" as InterfacesProduct
InterfacesProduct {
    name: "interfaces"
    files: [
        "*.h"
    ]

    Depends { name: "Qt"; submodules: ["widgets", "sql"] }
    Depends { name: "sqlextension" }

    FileTagger {
        patterns: "*.h"
        fileTags: ["interfaces"]
    }

    Group {
        fileTagsFilter: "interfaces"
        qbs.install: true
        qbs.installDir: product.includePath + "/" + "interfaces"
    }
}
