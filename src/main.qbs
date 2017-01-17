import "../main.qbs" as MainProduct
MainProduct {
   name: "src"
   cpp.includePaths: base.concat([
         project.sourceDirectory + "/src/libs/",
         project.sourceDirectory + "/src/shared/",
         project.sourceDirectory + "/src/plugins/",
         project.sourceDirectory + "/src/interfaces/"
   ])
}
