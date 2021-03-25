add_library(types_lib 
  type.hh
  type.cc
  type_container.hh
  type_container.cc
)

target_include_directories(types_lib INTERFACE ..)
