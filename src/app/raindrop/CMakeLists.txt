cmake_minimum_required(VERSION 3.10)
project(raindrop VERSION 1.0.0)

set(PRODUCTNAME "Raindrop")
set(DESCRIPTION "Main program")
set(COMPANY     "RTPTechGroup")
set(COPYRIGHT   "Copyright (C) RTPTechGroup")
set(LIBRARIES
    plugin
)

include("${CMAKE_SOURCE_DIR}/cmake/app.cmake")

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

find_package(Qt5Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
