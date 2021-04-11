add_executable(lexer_tests
  parsing_trie_tests.cc
)

target_link_libraries(lexer_tests PRIVATE lexer_lib)
target_link_libraries(lexer_tests PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_add_tests(TARGET lexer_tests)