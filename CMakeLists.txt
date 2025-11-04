cmake_minimum_required(VERSION 3.29)
project(HyperRender VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

option(BUILD_TEST "Option to build tests" ON)

add_subdirectory(source)

if(BUILD_TEST)
#    add_subdirectory(test)
endif ()