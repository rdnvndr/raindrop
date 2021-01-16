cmake_minimum_required(VERSION 3.10)
project(raindrop VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(doc)

if ( ${CMAKE_TESTING_ENABLED} )
    add_subdirectory(tests)
endif()
