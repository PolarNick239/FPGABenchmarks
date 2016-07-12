cmake_minimum_required(VERSION 2.8.11)

project(demos)

add_executable(show_devices src/show_devices/main.cpp)
target_link_libraries(show_devices cl_utils)
