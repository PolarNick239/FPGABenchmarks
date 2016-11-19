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
        src/mandelbrot_ispc/mandelbrot_ispc_sse.cpp
        src/mandelbrot_ispc/mandelbrot_ispc_avx.cpp
        )

set(SPMD_HEADERS
        src/mandelbrot_ispc/gen/mandelbrotProcess_sse.h
        src/mandelbrot_ispc/gen/mandelbrotProcess_avx.h
        )

set(SPMD_OBJECTS
        src/mandelbrot_ispc/gen/mandelbrotProcess_sse.o
        src/mandelbrot_ispc/gen/mandelbrotProcess_avx.o
        )

convertIntoHeader(src/mandelbrot_ocl/mandelbrot.cl src/mandelbrot_ocl/mandelbrot_cl.h mandelbrot_kernel)

if(SPMD_SUPPORT)
    add_definitions(-DSPMD_SUPPORT)
    list(APPEND HEADERS ${SPMD_HEADERS})

    spmdCompile(src/mandelbrot_ispc/mandelbrot.ispc src/mandelbrot_ispc/gen mandelbrotProcess sse sse4)
    spmdCompile(src/mandelbrot_ispc/mandelbrot.ispc src/mandelbrot_ispc/gen mandelbrotProcess avx avx2)

    SET_SOURCE_FILES_PROPERTIES(|
            ${SPMD_HEADERS}
            PROPERTIES
            GENERATED true
    )

    SET_SOURCE_FILES_PROPERTIES(
            ${SPMD_OBJECTS}
            PROPERTIES
            EXTERNAL_OBJECT true
            GENERATED true
    )

    add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${SPMD_OBJECTS})
else()
    add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} cl_utils images primitives commons cpuid)
target_include_directories(${PROJECT_NAME} PUBLIC include)


add_executable(mandelbrot_demo src/mandelbrot_demo.cpp ${HEADERS})
target_link_libraries(mandelbrot_demo ${PROJECT_NAME})


add_executable(mandelbrot_bench src/mandelbrot_bench.cpp ${HEADERS})
target_link_libraries(mandelbrot_bench ${PROJECT_NAME})
