add_library(parser_lib   
  generated/Parser_generated.h
  generated/Parser_generatedbase.h
  generated/parse.cc
  lexer_adaptor.cc lexer_adaptor.hh
)

add_executable(parser_driver   
  parser_driver.cc
)

target_include_directories(parser_lib INTERFACE ..)

target_link_libraries(parser_lib PRIVATE parser_ast_lib)
target_link_libraries(parser_lib PRIVATE lexer_lib)

target_link_libraries(parser_driver PRIVATE parser_lib)