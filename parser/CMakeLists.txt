add_library(parser_lib EXCLUDE_FROM_ALL
  parser.hh
  parser.cc
)

add_executable(parser_driver EXCLUDE_FROM_ALL
  parser_driver.cc
)

target_include_directories(parser_lib INTERFACE ..)

target_link_libraries(parser_lib PRIVATE parser_ast_lib)
target_link_libraries(parser_lib PRIVATE lexer_lib)

target_link_libraries(parser_driver PRIVATE parser_lib)