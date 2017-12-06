cmake_minimum_required (VERSION 2.8)
project (depp)

set(CMAKE_CXX_COMPILER clang++)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "--std=c++17 -stdlib=libc++ -g -Wall -Wno-c++98-compat -Wno-global-constructors \
         -Wno-exit-time-destructors -Wno-zero-as-null-pointer-constant \
         -Wno-padded -Wno-unused-macros")
set(CMAKE_SHARED_LINKER_FLAGS "-l caf_core")

include_directories(${CMAKE_SOURCE_DIR}/)
add_subdirectory(src)
add_subdirectory(test)
enable_testing()
add_test(NAME MyTest COMMAND Test)
