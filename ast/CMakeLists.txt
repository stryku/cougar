add_library(ast_lib
  node.cc node.hh
  module.cc module.hh
  function.cc function.hh
  type.cc type.hh
)

target_include_directories(ast_lib INTERFACE ..)

target_link_libraries(ast_lib PUBLIC utils_lib)
