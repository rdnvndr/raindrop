import qbs

Product {
    property string productname: ""
    property string version:     "1"
    property string description: ""
    property string company:     ""
    property string copyright:   ""

    property string applicationPath: "bin"
    property string interfacePath:   "include/raindrop/"
    property string rPluginPath:     "../lib/raindrop/plugins/"
    Properties {
        condition: qbs.targetOS.contains("linux")
        property string rLibraryPath:    "../lib/raindrop"
    }
    Properties {
        condition: qbs.targetOS.contains("windows")
        property string rLibraryPath:    ""
    }

    cpp.rpaths: "../lib/raindrop"
    cpp.defines: [
        "APP_VERSION="     + "\"" +  product.version + "\"",
        "APP_COMPANY="     + "\"" +  product.company + "\"",
        "APP_PRODUCT="     + "\"" +  product.productname + "\"",
        "APP_DESCRIPTION=" + "\"" +  product.description + "\"",
        "APP_COPYRIGHT="   + "\"" +  product.copyright + "\"",
        "APP_NAME="        + "\"" +  product.name + "\""
    ]
    Depends { name: "cpp" }
}
