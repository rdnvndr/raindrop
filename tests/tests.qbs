Project {
    condition: project.withTests
    references: [
        "auto/auto.qbs",
        "benchmarks/benchmarks.qbs",
        "manual/manual.qbs"
    ]
}
