import "../main.qbs" as InterfacesProduct
InterfacesProduct {
    name: "interfaces"
    files: [
        "iclipboardelement.h",
        "iclipboarditem.h",
        "iclipboardstack.h",
        "idatabaseattribute.h",
        "idatabaseclass.h",
        "idatabasecomposition.h",
        "idatabaseexpression.h",
        "idatabasefilter.h",
        "idatabaseitem.h",
        "idatabasemodelbuilder.h",
        "idatabasemodel.h",
        "idatabaserole.h",
        "imainwindow.h",
        "itreedockwidget.h",
        "iundogroup.h"
    ]

    Depends { name: "Qt"; submodules: ["widgets"] }

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
