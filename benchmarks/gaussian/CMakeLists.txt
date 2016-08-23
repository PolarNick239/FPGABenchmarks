cmake_minimum_required(VERSION 2.8.11)

project(gaussian)

set(HEADERS
        include/gaussian.h
        )

set(SOURCES
        src/gaussian.cpp
        src/gaussian_cpu.cpp
        src/gaussian_single_thread.cpp
        )

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} cl_utils images primitives commons cpuid)
target_include_directories(${PROJECT_NAME} PUBLIC include)


add_executable(gaussian_demo src/gaussian_demo.cpp ${SOURCES} ${HEADERS})
target_link_libraries(gaussian_demo ${PROJECT_NAME})


add_executable(gaussian_bench src/gaussian_bench.cpp ${SOURCES} ${HEADERS})
target_link_libraries(gaussian_bench ${PROJECT_NAME})
