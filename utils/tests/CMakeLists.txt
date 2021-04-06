add_executable(utils_tests
  utf8_decoder_tests.cc
)

target_link_libraries(utils_tests PRIVATE utils_lib)
target_link_libraries(utils_tests PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_add_tests(TARGET utils_tests)