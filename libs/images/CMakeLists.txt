cmake_minimum_required(VERSION 2.8.11)

project(images)

# This is thin wrapper around CImg header-only library available at https://github.com/dtschump/CImg

find_package(Threads REQUIRED)
find_package(X11 REQUIRED)

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/CImg/CImg.h")
    message(SEND_ERROR "CImg library submodule was not downloaded! Please, execute:
        git submodule update --init")
endif()

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/CImg)

set(HEADERS
        include/images.h
        )

set(SOURCES
        src/images.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
