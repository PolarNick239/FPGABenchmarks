cmake_minimum_required(VERSION 2.8.11)

project(fpga_playground)

find_package(OpenMP REQUIRED)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra -Wno-unused-function -Wno-unused-parameter")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -msse -msse2 -mssse3 -mavx2")
endif()

function(convertIntoHeader sourceFile headerFile arrayName)
    add_custom_command (
            OUTPUT
            ${headerFile}

            COMMAND xxd -ps ${PROJECT_SOURCE_DIR}/${sourceFile} | fold -w2 | paste -sd' '| sed -e 's/ /, 0x/g' | printf \"0x$$\(cat -\)\" | fold -w120 | printf \"static const char ${arrayName}[] = {\\n$$\(cat -\)}\$<SEMICOLON>\\n\\nsize_t ${arrayName}_length = sizeof\(${arrayName}\) / sizeof\(unsigned char\)$<SEMICOLON>\\n\" > ${PROJECT_SOURCE_DIR}/${headerFile}

            DEPENDS
            ${sourceFile}
    )
endfunction()

add_subdirectory(thirdparty)
add_subdirectory(libs)
add_subdirectory(benchmarks)
