import "../main.qbs" as LibProduct
LibProduct {
    name: "plugin"
    Group {
        fileTagsFilter: ["application"]
        qbs.install: true
        qbs.installDir: "tests/auto/plugin/"
    }
}
