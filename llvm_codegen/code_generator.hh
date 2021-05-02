#pragma once

#include <memory>

namespace llvm {
class LLVMContext;
class Module;
} // namespace llvm

namespace Cougar::Ast {
class Module;
}

namespace Cougar::LlvmCodeGenerator {

class ModuleWrapper;

class CodeGenerator {
public:
  CodeGenerator();
  ~CodeGenerator();

  ModuleWrapper generate(Ast::Module &module);

  void compile(ModuleWrapper &module);
  void dumpIR(ModuleWrapper &module);

private:
  std::unique_ptr<llvm::LLVMContext> mContext;
};

// type-hiding unique pointer to llvm::Module
class ModuleWrapper {
public:
  ModuleWrapper(llvm::Module *m);
  ModuleWrapper(const ModuleWrapper &) = delete;
  ~ModuleWrapper();

private:
  llvm::Module *mModule = nullptr;
};

} // namespace Cougar::LlvmCodeGenerator
