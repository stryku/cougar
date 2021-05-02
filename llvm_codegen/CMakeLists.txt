add_library(llvm_codegen_lib
  code_generator.cc code_generator.hh

  generate_function.cc
)

target_link_libraries(llvm_codegen_lib ast_lib)
target_link_libraries(llvm_codegen_lib meta_lib)

target_include_directories(llvm_codegen_lib SYSTEM PRIVATE  /usr/lib/llvm-11/include)
target_link_directories(llvm_codegen_lib PUBLIC /usr/lib/llvm-11/lib)

target_link_libraries(llvm_codegen_lib LLVM)
target_link_libraries(llvm_codegen_lib fmt::fmt)
