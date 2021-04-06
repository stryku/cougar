add_executable(llvm_hello_world EXCLUDE_FROM_ALL
  llvm_hello_world.cc
)

target_include_directories(llvm_hello_world SYSTEM PRIVATE  /usr/lib/llvm-11/include)
target_link_directories(llvm_hello_world PRIVATE /usr/lib/llvm-11/lib)

target_link_libraries(llvm_hello_world PRIVATE LLVM)
target_link_libraries(llvm_hello_world PRIVATE fmt::fmt)
