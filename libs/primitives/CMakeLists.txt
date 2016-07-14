cmake_minimum_required(VERSION 2.8.11)

project(primitives)

set(HEADERS
        include/vector2.h
        )

set(SOURCES
        src/vector2.cpp
        )

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
