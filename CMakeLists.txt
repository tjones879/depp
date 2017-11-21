cmake_minimum_required (VERSION 2.8)
project (depp)
set(CMAKE_CXX_COMPILER clang++)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "--std=c++17 -stdlib=libc++ -g -Wall -Wno-c++98-compat -Wno-global-constructors \
         -Wno-exit-time-destructors -Wno-zero-as-null-pointer-constant \
         -Wno-padded -Wno-unused-macros")
set(CMAKE_SHARED_LINKER_FLAGS "-l caf_core")

find_package(BISON REQUIRED)
BISON_TARGET(MyParser ${CMAKE_SOURCE_DIR}/src/parser.y ${CMAKE_SOURCE_DIR}/gen/parser.cpp)

find_package(FLEX REQUIRED)
FLEX_TARGET(MyScanner ${CMAKE_SOURCE_DIR}/src/scanner.l ${CMAKE_SOURCE_DIR}/gen/lexer.cpp)

include_directories("inc/")
add_executable(main
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
    ${SRC_DIR}/acting.cpp
    ${SRC_DIR}/ast.cpp
    ${SRC_DIR}/env.cpp
    ${SRC_DIR}/generator.cpp
)
