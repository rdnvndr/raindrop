import "../main.qbs" as SrcProduct
SrcProduct {
   name: "libs"
   type: "dynamiclibrary"
   Export {
       Depends { name: "cpp" }
       cpp.includePaths: product.sourceDirectory + "/../"
   }
}
