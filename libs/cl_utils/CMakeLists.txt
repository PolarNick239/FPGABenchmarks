cmake_minimum_required(VERSION 2.8.11)

project(cl_utils)

find_package(OpenCL REQUIRED)

set(HEADERS
        include/cl/common.h
        include/cl/Platform.h
        include/cl/Device.h
        include/cl/Kernel.h
        include/cl/Engine.h
        )

set(SOURCES
        src/cl/common.cpp
        src/cl/Platform.cpp
        src/cl/Device.cpp
        src/cl/Kernel.cpp
        src/cl/Engine.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${OpenCL_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC include)
