import "../main.qbs" as InterfacesProduct
InterfacesProduct {
    name: "interfaces"
    files: [
        "idatabaseattribute.h",
        "idatabaseclass.h",
        "idatabasecomposition.h",
        "idatabaseexpression.h",
        "idatabasefilter.h",
        "idatabaseitem.h",
        "idatabaselist.h",
        "idatabasemodelbuilder.h",
        "idatabasemodel.h",
        "idatabaserole.h",
        "imainwindow.h",
        "itreedockwidget.h",
        "iundogroup.h",
        "iclipboardstack.h",
        "iclipboarditem.h",
        "iclipboardelement.h",
        "idatabaseconnect.h",
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
