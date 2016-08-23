cmake_minimum_required(VERSION 2.8.11)

project(commons)

set(HEADERS
        include/timer.h
        include/omp_commons.h
        include/type_helpers.h
        )

set(SOURCES
        src/timer.cpp
        )

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
