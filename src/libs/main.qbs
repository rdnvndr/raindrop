import "../main.qbs" as SrcProduct
SrcProduct {
   name: "libs"
   type: "dynamiclibrary"
   Export {
       Depends { name: "cpp" }
       cpp.includePaths: product.sourceDirectory + "/../"
   }

   Group {
       fileTagsFilter: [
           "dynamiclibrary",
           "dynamiclibrary_symlink",
           "dynamiclibrary_import"
       ]
       qbs.install: true
       qbs.installDir: product.applicationPath + "/" + product.rLibraryPath
   }

   FileTagger {
           patterns: "*.h"
           fileTags: ["include"]
   }
   Group {
       fileTagsFilter: "include"
       qbs.install: true
       qbs.installDir: includePath + "/" + product.name
   }
}
