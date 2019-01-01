cmake_minimum_required(VERSION 3.10)
project(example VERSION 1.0.0)

set(PRODUCTNAME "Пример")
set(DESCRIPTION "Example Plugin")
set(COMPANY     "RTPTechGroup")
set(COPYRIGHT   "Copyright (C) RTPTechGroup")

set(LIBRARIES
    plugin
    interfaces
)

include("${CMAKE_SOURCE_DIR}/cmake/plugins.cmake")

target_compile_definitions(${PROJECT_NAME} PRIVATE -DEXAMPLE_LIBRARY)

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

find_package(Qt5Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)