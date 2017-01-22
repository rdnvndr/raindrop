import qbs.FileInfo
import "../main.qbs" as MainProduct
MainProduct {
   name: "tests"
   cpp.rpaths: rLibraryTestPath
}
