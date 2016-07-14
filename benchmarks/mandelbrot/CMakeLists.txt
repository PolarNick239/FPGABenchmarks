cmake_minimum_required(VERSION 2.8.11)

project(mandelbrot)

set(HEADERS
        include/mandelbrot.h
        )

set(SOURCES
        src/mandelbrot_cpu.cpp
        )

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} cl_utils images primitives)
target_include_directories(${PROJECT_NAME} PUBLIC include)
