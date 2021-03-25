add_library(ast_lib 
  ast.hh
  ast.cc
)

target_include_directories(ast_lib INTERFACE ..)

target_link_libraries(ast_lib PRIVATE types_lib)

target_link_libraries(ast_lib PRIVATE fmt::fmt)
