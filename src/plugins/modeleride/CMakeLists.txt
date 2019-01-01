cmake_minimum_required(VERSION 3.10)
project(modeleride VERSION 1.0.0)

set(PRODUCTNAME "Редактор модели данных")
set(DESCRIPTION "ModelerIDE Plugin")
set(COMPANY     "RTPTechGroup")
set(COPYRIGHT   "Copyright (C) RTPTechGroup")

set(LIBRARIES
    plugin
    guiextension
    metadatamodel
    interfaces
)

include("${CMAKE_SOURCE_DIR}/cmake/plugins.cmake")

target_compile_definitions(${PROJECT_NAME} PRIVATE -DMAINWINDOW_LIBRARY)

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

find_package(Qt5Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

find_package(Qt5Xml)
target_link_libraries(${PROJECT_NAME} Qt5::Xml)

# Установка проекта
install(FILES "${PROJECT_SOURCE_DIR}/data/system.xml" DESTINATION "${DOCPATH}/${PROJECT_NAME}")
