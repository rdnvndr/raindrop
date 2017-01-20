import "../main.qbs" as Product
Product {
   name: "shared"
   type: "staticlibrary"
   Export {
       Depends { name: "cpp" }
       cpp.includePaths: product.sourceDirectory + "/../"
   }
}
