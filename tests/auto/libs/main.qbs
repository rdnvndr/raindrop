import "../main.qbs" as AutoTestProduct
AutoTestProduct {
   name: "libs"
   qbs.install: true
   qbs.installDir: "tests/auto"
}
