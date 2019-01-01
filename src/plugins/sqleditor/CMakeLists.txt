cmake_minimum_required(VERSION 3.10)
project(sqleditor VERSION 1.0.0)

set(PRODUCTNAME "SQL редактор")
set(DESCRIPTION "SqlEditor Plugin")
set(COMPANY     "RTPTechGroup")
set(COPYRIGHT   "Copyright (C) RTPTechGroup")

set(LIBRARIES
    plugin
    interfaces
)

include("${CMAKE_SOURCE_DIR}/cmake/plugins.cmake")

target_compile_definitions(${PROJECT_NAME} PRIVATE -DSQLEDITOR_LIBRARY)

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

find_package(Qt5Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

find_package(Qt5Sql)
target_link_libraries(${PROJECT_NAME} Qt5::Sql)
