import "../main.qbs" as TestProduct
TestProduct {
   name: "benchmarks"
   qbs.install: true
   qbs.installDir: "tests/bechmarks"
}
