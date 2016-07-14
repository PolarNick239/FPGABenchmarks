cmake_minimum_required(VERSION 2.8.11)

project(images)

# This is thin wrapper around CImg header-only library available at https://github.com/dtschump/CImg

find_package(Threads REQUIRED)
find_package(X11 REQUIRED)

set(HEADERS
        include/images.h
        )

set(SOURCES
        src/images.cpp
        )

add_library(images ${SOURCES} ${HEADERS})

target_link_libraries(images ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES})
target_include_directories(images PUBLIC include)
