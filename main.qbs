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
    property string includePath:     "include/raindrop/"
    property string docPath:         "share/doc/raindrop"
    property string rPluginPath:     "../lib/raindrop/plugins/"

    Properties {
        condition: qbs.targetOS.contains("linux")
        property string rLibraryPath:     "../lib/raindrop"
        property string rLibraryTestPath: "../../../lib/raindrop"
    }
    Properties {
        condition: qbs.targetOS.contains("windows")
        property string rLibraryPath:     ""
        property string rLibraryTestPath: ""
    }

    cpp.defines: [
        "APP_VERSION="     + "\"" +  product.version + "\"",
        "APP_COMPANY="     + "\"" +  product.company + "\"",
        "APP_PRODUCT="     + "\"" +  product.productname + "\"",
        "APP_DESCRIPTION=" + "\"" +  product.description + "\"",
        "APP_COPYRIGHT="   + "\"" +  product.copyright + "\"",
        "APP_NAME="        + "\"" +  product.name + "\""
    ]
    Depends { name: "cpp" }

    Probe {
        id: version
        property stringList targetOS: qbs.targetOS
        property string prjDir: project.sourceDirectory
        property string prdDir: product.sourceDirectory
        property int hack: {
            //A bit of a hack to make qbs re-resolve (see QBS-996)
            return  File.lastModified(prjDir + "/.git/logs/HEAD");
        }
        configure: {
            var cmd;
            var args;
            if (targetOS.contains("windows")) {
                cmd = "cmd";
                args = ["/c", prjDir+"/scripts/version.bat"];
            }
            if (targetOS.contains("linux")) {
                cmd = "/usr/bin/sh";
                args = ["-c", prjDir+"/scripts/version.sh"];
            }

            var p = new Process();
            p.setWorkingDirectory(prdDir)
            if (0 === p.exec(cmd, args)) {
                found = true;
            } else {
                found = false;
            }
            p.close();
        }
    }
}
