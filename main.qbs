import qbs
import qbs.Process
import qbs.File

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

    readonly property string prjDir: project.sourceDirectory
    Probe {
        configure: {
            //A bit of a hack to make qbs re-resolve (see QBS-996)
            var hack = File.lastModified(prjDir + "/.git/logs/HEAD")

            var cmd;
            var args;
            if (qbs.targetOS.contains("windows")) {
                cmd = "cmd";
                args = ["/c", prjDir+"/scripts/version.bat"];
            } else {
                cmd = prjDir+"/scripts/version.sh";
                args = [];
            }
            var p = new Process();
            p.setWorkingDirectory(sourceDirectory)
            if (0 === p.exec(cmd, args)) {
                found = true;
            } else {
                found = false;
            }
            p.close();
        }
    }
}