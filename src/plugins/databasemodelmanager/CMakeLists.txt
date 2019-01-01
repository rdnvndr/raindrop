cmake_minimum_required(VERSION 3.10)
project(databasemodelmanager VERSION 1.0.0)

set(PRODUCTNAME "Менеджер моделей баз данных")
set(DESCRIPTION "Менеджер моделей баз данных")
set(COMPANY     "RTPTechGroup")
set(COPYRIGHT   "Copyright (C) RTPTechGroup")

set(LIBRARIES
    plugin
    sqlextension
    interfaces
)

include("${CMAKE_SOURCE_DIR}/cmake/plugins.cmake")

target_compile_definitions(${PROJECT_NAME} PRIVATE -DDATABASEMODELMANAGER_LIBRARY)

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

find_package(Qt5Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

find_package(Qt5Sql)
target_link_libraries(${PROJECT_NAME} Qt5::Sql)
