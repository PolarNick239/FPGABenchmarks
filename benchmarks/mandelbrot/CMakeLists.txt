cmake_minimum_required(VERSION 2.8.11)

project(mandelbrot)

set(HEADERS
        include/mandelbrot.h
        )

set(SOURCES
        src/mandelbrot_cpu.cpp
        )

add_executable(mandelbrot src/main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(mandelbrot cl_utils images primitives)
target_include_directories(mandelbrot PUBLIC include)
