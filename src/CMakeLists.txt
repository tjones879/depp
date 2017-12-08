set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

find_package(BISON REQUIRED)
BISON_TARGET(MyParser ${CMAKE_SOURCE_DIR}/src/parser.y ${CMAKE_SOURCE_DIR}/gen/parser.cpp)

find_package(FLEX REQUIRED)
FLEX_TARGET(MyScanner ${CMAKE_SOURCE_DIR}/src/scanner.l ${CMAKE_SOURCE_DIR}/gen/lexer.cpp)

include_directories(${CMAKE_SOURCE_DIR}/gen)

add_library(depp
    ${FLEX_MyScanner_OUTPUTS}
    ${SRC_DIR}/ast.cpp
    ${SRC_DIR}/builtin.cpp
    ${SRC_DIR}/env.cpp
    ${SRC_DIR}/generator.cpp
    ${BISON_MyParser_OUTPUTS})

add_executable(run main.cpp)

target_link_libraries(run depp)
