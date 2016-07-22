cmake_minimum_required(VERSION 2.8.11)

project(fpga_playground)

find_package(OpenMP REQUIRED)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra -Wno-unused-function")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -msse -msse2 -mssse3 -mavx2")
endif()

add_subdirectory(thirdparty)
add_subdirectory(libs)
add_subdirectory(benchmarks)
