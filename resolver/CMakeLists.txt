add_library(resolver_lib
  resolver.hh resolver.cc
)

add_executable(resolver_driver
  resolver_driver.cc
)

target_include_directories(resolver_lib INTERFACE ..)

target_link_libraries(resolver_lib PRIVATE ast_lib)
target_link_libraries(resolver_lib PRIVATE lexer_lib)

target_link_libraries(resolver_driver PRIVATE resolver_lib parser_lib lexer_lib)