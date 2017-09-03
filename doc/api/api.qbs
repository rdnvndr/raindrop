import "../main.qbs" as DocProduct

DocProduct {
    name: "raindrop-api"
    type: "docs"

    docName: "raindrop-dev"
    Group {
        fileTagsFilter: ["docs"]
        qbs.install: true
        qbs.installDir: product.docPath
    }
}
