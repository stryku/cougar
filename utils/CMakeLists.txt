add_library(utils_lib
  utf8_decoder.hh utf8_decoder.cc
  zone_allocator.hh zone_allocator.cc
)

target_include_directories(utils_lib INTERFACE ..)

add_subdirectory(tests)

