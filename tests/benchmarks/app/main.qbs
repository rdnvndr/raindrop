import "../main.qbs" as BenchmarkProduct
BenchmarkProduct {
    name: "app"
    type: "application"
    Group {
        fileTagsFilter: ["application"]
        qbs.install: true
        qbs.installDir: "tests/benchmarks/" + product.applicationPath  + "/"
    }
}
