add_library(utils_lib
  utf8_decoder.hh utf8_decoder.cc
  zone_allocator.hh zone_allocator.cc
  file_loader.hh file_loader.cc
  list.hh list.cc
  source_location.hh source_location.cc
  diagnostics.hh diagnostics.cc
)

target_include_directories(utils_lib INTERFACE ..)

target_link_libraries(utils_lib PRIVATE fmt::fmt)


add_subdirectory(tests)

