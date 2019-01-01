include("${CMAKE_SOURCE_DIR}/cmake/main.cmake")

cmake_minimum_required(VERSION 3.10)
project(interfaces VERSION 1.0.0)

set(PRODUCTNAME "Interfaces")
set(DESCRIPTION "Interfaces")
set(COMPANY     "RTPTechGroup")
set(COPYRIGHT   "Copyright (C) RTPTechGroup")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
    INTERFACE ${PROJECT_SOURCE_DIR}
)

# Установка проекта
file(GLOB PUBHEADERS "${PROJECT_SOURCE_DIR}/*.h")
install(FILES ${PUBHEADERS}  DESTINATION "${INCPATH}/${PROJECT_NAME}")
