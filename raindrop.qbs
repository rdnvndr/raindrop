import qbs
Project {
    minimumQbsVersion: qbs.version
    property bool withTests: false
    references: [
        "src/src.qbs",
        "tests/tests.qbs"
    ]
}
