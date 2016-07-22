cmake_minimum_required(VERSION 2.8.11)

project(cpuid)

set(CPUID_DIR "${PROJECT_SOURCE_DIR}/cpuid/src/cpuid")

if (NOT EXISTS "${CPUID_DIR}/cpuinfo.hpp")
    message(SEND_ERROR "cpuid library submodule was not downloaded! Please, execute:
        git submodule update --init")
endif()

include_directories(include)

set(HEADERS
        include/cpuinfo.h
        ${CPUID_DIR}/cpuinfo.hpp
        )

set(SOURCES
        ${CPUID_DIR}/cpuinfo.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC include ${CPUID_DIR})
