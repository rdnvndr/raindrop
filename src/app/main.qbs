import "../main.qbs" as SrcProduct
SrcProduct {
   name: "app"
   type: "application"
   Group {
       fileTagsFilter: ["application"]
       qbs.install: true
       qbs.installDir: product.applicationPath
   }
}
