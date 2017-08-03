import "../main.qbs" as LibProduct
LibProduct {
    name: "sqlextension"
    Group {
        fileTagsFilter: ["application"]
        qbs.install: true
        qbs.installDir: "tests/manual/sqlextension/"
    }
}
