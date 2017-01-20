import "../main.qbs" as TestProduct
TestProduct {
   name: "manual"
   qbs.install: true
   qbs.installDir: "tests/manual"
}
