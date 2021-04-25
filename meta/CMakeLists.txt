add_library(meta_lib

  type_info.cc type_info.hh
  scope.cc scope.hh
  function_info.cc function_info.hh
)

target_include_directories(meta_lib INTERFACE ..)

target_link_libraries(meta_lib PUBLIC utils_lib)
