add_library(lexer_lib EXCLUDE_FROM_ALL
    input_source.hh input_source.cc
    token.hh token.cc
    lexer.hh lexer.cc
)

add_executable(lexer_driver
    lexer_driver.cc
)

target_link_libraries(lexer_lib PRIVATE utils_lib)

target_include_directories(lexer_lib INTERFACE ..)

target_link_libraries(lexer_driver PRIVATE lexer_lib)
