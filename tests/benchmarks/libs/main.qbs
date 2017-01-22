import "../main.qbs" as BenchmarkProduct

BenchmarkProduct {
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
       qbs.installDir: "tests/benchmarks/" + product.applicationPath + "/"  + product.rLibraryPath+ "/"
   }

}
