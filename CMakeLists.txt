cmake_minimum_required(VERSION 3.29)
project(HyperRender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(HYPER_GPU_PATH ../HyperGpu)

#include_directories(${PROJECT_NAME} PUBLIC ${HYPER_GPU_PATH}/include)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp )
add_library(${PROJECT_NAME} SHARED ${SOURCE})


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC HyperGpu)