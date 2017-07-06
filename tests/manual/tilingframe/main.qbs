import "../main.qbs" as LibProduct
LibProduct {
    name: "tilingframe"
    Group {
        fileTagsFilter: ["application"]
        qbs.install: true
        qbs.installDir: "tests/manual/tilingframe/"
    }
}
