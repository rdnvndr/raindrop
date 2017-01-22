import "../main.qbs" as LibProduct
LibProduct {
   name: "plugin"
   cpp.rpaths: "../" + rLibraryTestPath
}
