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
        src/mandelbrot_ocl/mandelbrot_cl.h
        src/mandelbrot_ocl/mandelbrot_ocl.cpp
        )

convertIntoHeader(src/mandelbrot_ocl/mandelbrot.cl src/mandelbrot_ocl/mandelbrot_cl.h mandelbrot_kernel)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} cl_utils images primitives commons cpuid)
target_include_directories(${PROJECT_NAME} PUBLIC include)


add_executable(mandelbrot_demo src/mandelbrot_demo.cpp ${SOURCES} ${HEADERS})
target_link_libraries(mandelbrot_demo ${PROJECT_NAME})


add_executable(mandelbrot_bench src/mandelbrot_bench.cpp ${SOURCES} ${HEADERS})
target_link_libraries(mandelbrot_bench ${PROJECT_NAME})
