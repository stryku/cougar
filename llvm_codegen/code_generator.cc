#include "code_generator.hh"

#include "ast/module.hh"

#include "utils/overloaded.hh"

// #include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
// #include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>

// #include <llvm/Support/FileSystem.h>
// #include <llvm/Support/Host.h>
// #include <llvm/Support/TargetRegistry.h>
// #include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

// #include <llvm/Target/TargetMachine.h>
// #include <llvm/Target/TargetOptions.h>

#include <fmt/format.h>

namespace Cougar::LlvmCodeGenerator {

ModuleWrapper::ModuleWrapper(llvm::Module *m) : mModule(m) {}
ModuleWrapper::~ModuleWrapper() { delete mModule; }

CodeGenerator::CodeGenerator() {
  mContext = std::make_unique<llvm::LLVMContext>();
}

CodeGenerator::~CodeGenerator() = default;

ModuleWrapper CodeGenerator::generate(Ast::Module &moduleAST) {

  ModuleWrapper module(new llvm::Module(moduleAST.moduleName(), *mContext));

  for (Ast::ModuleStatement &stmt : moduleAST.statements()) {
    stmt.visit(overloaded{
        [&](Ast::FunctionDeclaration *fd) { generateFunction(*fd, *module); },
        [](auto &) {}});
  }

  return module;
}

void CodeGenerator::compile(ModuleWrapper &module) {
  (void)module;

  throw std::logic_error("Compilation not impelmented");
}

void CodeGenerator::dumpIR(ModuleWrapper &module) {
  module->print(llvm::errs(), nullptr);
}

} // namespace Cougar::LlvmCodeGenerator