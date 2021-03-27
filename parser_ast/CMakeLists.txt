add_library(parser_ast_lib 
  parser_ast.hh
  parser_ast.cc
)

target_include_directories(parser_ast_lib INTERFACE ..)

target_link_libraries(parser_ast_lib PRIVATE fmt::fmt)
