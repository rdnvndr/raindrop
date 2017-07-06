import "../main.qbs" as LibProduct
LibProduct {
    name: "treexmlmodel"
    Group {
        fileTagsFilter: ["application"]
        qbs.install: true
        qbs.installDir: "tests/manual/treexmlmodel/"
    }
}
