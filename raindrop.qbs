import qbs
Project {
    minimumQbsVersion: qbs.version
    references: [
        "src/src.qbs",
        "tests/tests.qbs"
    ]
}
