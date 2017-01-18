import "../main.qbs" as SrcProduct
SrcProduct {
   name: "plugins"
   type: "dynamiclibrary"
   Export {
       Depends { name: "cpp" }
       cpp.includePaths: product.sourceDirectory + "/../"
   }

   qbs.install: true
   qbs.installDir: "plugins"
}
