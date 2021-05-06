#pragma once

#include <memory>

namespace llvm {
class LLVMContext;
class Module;
class Type;
class Function;
class IRBuilderBase;
class Value;
} // namespace llvm

namespace Cougar::Ast {
class Module;
class FunctionDeclaration;

class Statement;
struct StGroup;
struct StFunctionCall;

struct ExStringLiteral;

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
  void generateFunctionBody(llvm::Function *llvmFunction, Ast::StGroup &body);

  // statements
  void generateStatement(Ast::StGroup &grp);
  void generateStatement(Ast::StFunctionCall &);

  // expressions
  llvm::Value *generateExpression(Ast::ExStringLiteral &);

  // types
  llvm::Type *simpleTypeToLlvm(std::string_view name);
  llvm::Type *toLlvm(Meta::TypeInfo *ti);
  llvm::Type *pointerTypeToLlvm(Meta::TypeInfo *pointed);

  std::unique_ptr<llvm::LLVMContext> mContext;

  // TODO these should be in 'module builder' object
  // only valid during a call to generate
  llvm::IRBuilderBase *mBuilder = nullptr;
  llvm::Module *mModule = nullptr;
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
  llvm::Module *get() { return mModule; }

private:
  llvm::Module *mModule = nullptr;
};

} // namespace Cougar::LlvmCodeGenerator
