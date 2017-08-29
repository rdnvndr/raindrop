import "../main.qbs" as DocProduct

DocProduct {
    name: "api"
    type: "docs"

    docName: "raindrop-dev"
    Group {
        fileTagsFilter: ["docs"]
        qbs.install: true
        qbs.installDir: product.docPath
    }
}
