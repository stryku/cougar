add_executable(cougar
  driver.cc
)

target_link_libraries(cougar resolver_lib)
target_link_libraries(cougar parser_lib)
target_link_libraries(cougar lexer_lib)