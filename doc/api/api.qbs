import qbs;
import qbs.Process;
import "../main.qbs" as DocProduct

DocProduct {
    name: "api"
    type: "api"

    FileTagger {
        fileTags: ["api"]
        patterns: ["*.qch"]
    }

    Group {
        name: "markdown"
        files: ["pages/*.markdown"]
        fileTags: ["markdown"]
    }

    Group {
        name: "doxygen"
        files: ["Doxyfile"]
        fileTags: ["doxyconf"]
    }

    Group {
        fileTagsFilter: ["api"]
        qbs.install: true
        qbs.installDir: product.docPath + "/api/"
    }

    Rule {
        multiplex: true
        inputs: ["doxyconf"]
        outputFileTags: ["api"]
        outputArtifacts: [{ filePath: "raindrop.qch" }, { filePath: "html/" }]
        prepare: {
            var cmd = new JavaScriptCommand ();
            cmd.description = "generating api documentation";
            cmd.highlight   = "doxygen";
            cmd.sourceCode  = function () {
                for (var idx = 0; idx < inputs["doxyconf"].length; idx++) {
                    var file = inputs["doxyconf"][idx].filePath;
                    var proc    = new Process ();
                    proc.setWorkingDirectory (product.buildDirectory);
                    proc.exec ("doxygen", [file]);
                }
            }
            return cmd;
        }

    }
}
