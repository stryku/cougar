#include "code_generator.hh"

#include "ast/module.hh"

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <fmt/format.h>

namespace Cougar::LlvmCodeGenerator {

ModuleWrapper::ModuleWrapper(llvm::Module *m) : mModule(m) {}
ModuleWrapper::~ModuleWrapper() { delete mModule; }

CodeGenerator::CodeGenerator() {
  mContext = std::make_unique<llvm::LLVMContext>();
}

CodeGenerator::~CodeGenerator() = default;

ModuleWrapper CodeGenerator::generate(Ast::Module &moduleAST) {

  auto module =
      std::make_unique<llvm::Module>(moduleAST.moduleName(), *mContext);

  // TODO

  return ModuleWrapper(module.release());
}

void CodeGenerator::compile(ModuleWrapper &mwrap) {
  (void)mwrap;

  throw std::logic_error("Compilation not impelmented");
}

void CodeGenerator::dumpIR(ModuleWrapper &mwrap) {
  (void)mwrap;
  throw std::logic_error("IR dump impelmented");
}

} // namespace Cougar::LlvmCodeGenerator