cmake_minimum_required(VERSION 2.8.11)

project(utils)

find_package(OpenCL REQUIRED)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HEADERS
        include/cl/common.h
        include/cl/Platform.h
        include/cl/Device.h
        )

set(SOURCES
        src/cl/common.cpp
        src/cl/Platform.cpp
        src/cl/Device.cpp
        )

add_library(utils ${SOURCES} ${HEADERS})
target_link_libraries(utils ${OpenCL_LIBRARY})
target_include_directories(utils PUBLIC include)
