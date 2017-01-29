import "../main.qbs" as InterfacesProduct
InterfacesProduct {
    name: "interfaces"
    files: [
        "imainwindow.h",
        "itreedockwidget.h",
        "iundogroup.h",
        "iclipboardstack.h",
        "iclipboarditem.h",
        "iclipboardelement.h",
        "idatabaseconnect.h"
    ]

    FileTagger {
        patterns: "*.h"
        fileTags: ["interfaces"]
    }

    Group {
        fileTagsFilter: "interfaces"
        qbs.install: true
        qbs.installDir: product.interfacePath
    }
}
