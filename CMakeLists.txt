project(Cougar)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

find_package(fmt CONFIG REQUIRED)
#find_package(llvm REQUIRED)
find_package(Threads REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_subdirectory(lexer)
add_subdirectory(parser_ast)
add_subdirectory(parser)
add_subdirectory(types)
add_subdirectory(llvm_experiments)
add_subdirectory(utils)

