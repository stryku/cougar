add_library(parser_lib
  parser.hh parser.cc
)

add_executable(parser_driver
  parser_driver.cc
)

target_include_directories(parser_lib INTERFACE ..)

target_link_libraries(parser_lib PRIVATE ast_lib)
target_link_libraries(parser_lib PRIVATE lexer_lib)

target_link_libraries(parser_driver PRIVATE parser_lib)