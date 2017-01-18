import "../main.qbs" as MainProduct
MainProduct {
   name: "src"
   cpp.includePaths: base.concat([
         project.sourceDirectory + "/src/interfaces/"
   ])
}
