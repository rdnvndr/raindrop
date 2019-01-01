include("${CMAKE_SOURCE_DIR}/cmake/main.cmake")

# Места нахождения бинарных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}/${APPPATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}/${RLIBRARYPATH}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_DIR}/${RLIBRARYPATH}")

# Сборка проекта
include("${CMAKE_SOURCE_DIR}/cmake/rpath.cmake")
add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES} ${FORMS} ${RESOURCES})
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Установка проекта
install (TARGETS ${PROJECT_NAME} DESTINATION ${RLIBRARYPATH})
file(GLOB PUBHEADERS   "include/${PROJECT_NAME}/*.h")
foreach(PUBFILENAME ${PUBHEADERS})
    string(REPLACE "include/${PROJECT_NAME}/" "" FILENAME ${PUBFILENAME})
    install(FILES ${FILENAME} DESTINATION ${INCPATH})
endforeach()
