cmake_minimum_required(VERSION 2.8.11)

project(mandelbrot)

set(HEADERS
        include/mandelbrot.h
        )

set(SOURCES
        src/mandelbrot_cpu.cpp
        src/mandelbrot_cpu_sse.cpp
        src/mandelbrot_cpu_avx.cpp
        src/mandelbrot_single_thread.cpp
        )

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} cl_utils images primitives commons)
target_include_directories(${PROJECT_NAME} PUBLIC include)


add_executable(mandelbrot_demo src/main_demo.cpp ${SOURCES} ${HEADERS})
target_link_libraries(mandelbrot_demo ${PROJECT_NAME})


add_executable(mandelbrot_bench src/main_bench.cpp ${SOURCES} ${HEADERS})
target_link_libraries(mandelbrot_bench ${PROJECT_NAME})
