cmake_minimum_required(VERSION 2.8.11)

project(mandelbrot)

add_executable(mandelbrot src/main.cpp)

target_link_libraries(mandelbrot cl_utils images)
