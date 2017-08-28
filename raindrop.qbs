import qbs
Project {
    minimumQbsVersion: qbs.version
    references: [
        "src/src.qbs",
        "doc/doc.qbs",
        "tests/tests.qbs"
    ]
}
