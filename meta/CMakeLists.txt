add_library(meta_lib

  type_info.cc type_info.hh
  scope.cc scope.hh
  function_info.cc function_info.hh
  variable_info.cc variable_info.hh

  built_in.cc built_in.hh

)

target_include_directories(meta_lib INTERFACE ..)

target_link_libraries(meta_lib PUBLIC utils_lib)
