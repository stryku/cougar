add_library(lexer_lib EXCLUDE_FROM_ALL
    token.hh token.cc
    lexer.hh lexer.cc
    source_location.hh source_location.cc
)

add_executable(lexer_driver
    lexer_driver.cc
)

target_link_libraries(lexer_lib PRIVATE utils_lib)

target_include_directories(lexer_lib INTERFACE ..)

target_link_libraries(lexer_driver PRIVATE lexer_lib)
