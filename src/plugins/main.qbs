import "../main.qbs" as SrcProduct
SrcProduct {
   name: "plugins"
   type: "dynamiclibrary"
   Export {
       Depends { name: "cpp" }
       cpp.includePaths: product.sourceDirectory + "/../"
   }
   Group {
       fileTagsFilter: [
           "dynamiclibrary",
           "dynamiclibrary_symlink",
           "dynamiclibrary_import"]
       qbs.install: true
       qbs.installDir: "plugins"
   }
}
