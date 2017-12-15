cmake_minimum_required (VERSION 3.2)
project (depp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_COMPILER clang++)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "--std=c++17 -stdlib=libc++ -g -Wall -Wno-c++98-compat -Wno-global-constructors \
         -Wno-exit-time-destructors -Wno-zero-as-null-pointer-constant \
         -Wno-padded -Wno-unused-macros")

include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${CMAKE_SOURCE_DIR}/gen/)
add_subdirectory(src)
add_subdirectory(test)

# MISC Targets (clang-format)
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE ALL_HEADER_FILES inc/*.hpp inc/*.h)

find_program(FORMAT_PATH clang-format)
if(FORMAT_PATH)
    add_custom_target(format
        COMMAND ${FORMAT_PATH} -i ${ALL_SOURCE_FILES} ${ALL_HEADER_FILES})
endif()

find_program(TIDY_PATH clang-tidy)
if(TIDY_PATH)
    add_custom_target(tidy COMMAND ${TIDY_PATH} ${ALL_SOURCE_FILES} -fix -p ./)
endif()
