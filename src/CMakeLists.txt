set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

find_package(BISON REQUIRED)
BISON_TARGET(MyParser ${CMAKE_SOURCE_DIR}/src/parser.y ${CMAKE_SOURCE_DIR}/gen/parser.cpp)

find_package(FLEX REQUIRED)
FLEX_TARGET(MyScanner ${CMAKE_SOURCE_DIR}/src/scanner.l ${CMAKE_SOURCE_DIR}/gen/lexer.cpp)

add_library(depp 
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
    ${SRC_DIR}/acting.cpp
    ${SRC_DIR}/ast.cpp
    ${SRC_DIR}/builtin.cpp
    ${SRC_DIR}/env.cpp
    ${SRC_DIR}/generator.cpp)

add_executable(run ${SRC_DIR}/main.cpp)
target_link_libraries(run depp)
