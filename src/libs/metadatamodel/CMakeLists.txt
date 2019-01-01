cmake_minimum_required(VERSION 3.10)
project(metadatamodel VERSION 1.0.0)

set(PRODUCTNAME "MetadataModel Library")
set(DESCRIPTION "MetadataModel Library")
set(COMPANY     "RTPTechGroup")
set(COPYRIGHT   "Copyright (C) RTPTechGroup")

set(LIBRARIES
    treexmlmodel
)

include("${CMAKE_SOURCE_DIR}/cmake/libs.cmake")

target_compile_definitions(${PROJECT_NAME} PRIVATE -DMETADATAMODEL_LIBRARY)

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

find_package(Qt5Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
