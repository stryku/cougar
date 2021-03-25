add_executable(lexer 
    token.hh token.cc
    lexer.hh lexer.cc

    driver.cc

)

target_link_libraries(lexer PRIVATE fmt::fmt)
