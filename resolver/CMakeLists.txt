add_library(resolver_lib
  resolver.hh resolver.cc

  resolve_function.cc
  resolve_type.cc
  resolve_statement.cc
)

add_executable(resolver_driver
  resolver_driver.cc
)

target_include_directories(resolver_lib INTERFACE ..)

target_link_libraries(resolver_lib PRIVATE ast_lib)
target_link_libraries(resolver_lib PRIVATE meta_lib)

target_link_libraries(resolver_driver PRIVATE resolver_lib parser_lib lexer_lib)