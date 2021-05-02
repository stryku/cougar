#pragma once

#include <memory>

namespace llvm {
class LLVMContext;
class Module;
class Type;
} // namespace llvm

namespace Cougar::Ast {
class Module;
class FunctionDeclaration;
} // namespace Cougar::Ast

namespace Cougar::Meta {
class TypeInfo;
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
  // functions
  void generateFunction(Ast::FunctionDeclaration &funAST, llvm::Module &module);

  // types
  llvm::Type *simpleTypeToLlvm(std::string_view name);
  llvm::Type *toLlvm(Meta::TypeInfo *ti);
  llvm::Type *pointerTypeToLlvm(Meta::TypeInfo *pointed);

  std::unique_ptr<llvm::LLVMContext> mContext;
};

// type-hiding unique pointer to llvm::Module
class ModuleWrapper {
public:
  ModuleWrapper(llvm::Module *m);
  ModuleWrapper(const ModuleWrapper &) = delete;
  ModuleWrapper(ModuleWrapper &&) = default;
  ~ModuleWrapper();

  llvm::Module &operator*() { return *mModule; }
  llvm::Module *operator->() { return mModule; }

private:
  llvm::Module *mModule = nullptr;
};

} // namespace Cougar::LlvmCodeGenerator
