#include "code_generator.hh"

#include "ast/module.hh"

#include "utils/overloaded.hh"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>

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

  ModuleWrapper module(new llvm::Module(moduleAST.moduleName(), *mContext));
  llvm::IRBuilder<> builder(*mContext);
  mBuilder = &builder;
  mModule = module.get();

  for (Ast::ModuleStatement &stmt : moduleAST.statements()) {
    stmt.visit(overloaded{
        [&](Ast::FunctionDeclaration *fd) { generateFunction(*fd, *module); },
        [](auto &) {}});
  }

  mModule = nullptr;
  mBuilder = nullptr;

  return module;
}

void CodeGenerator::compile(ModuleWrapper &module) {

  auto targetTriple = llvm::sys::getDefaultTargetTriple();
  fmt::print("target triple: {}\n", targetTriple);

  // create compilation target
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::string error;
  auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!target) {
    throw std::runtime_error(
        fmt::format("Error intializing compilation target: {}", error));
  }

  // init machine
  std::string cpu = "generic";
  std::string features = "";
  llvm::TargetOptions opt;
  auto relocModel = llvm::Optional<llvm::Reloc::Model>();
  auto targetMachine =
      target->createTargetMachine(targetTriple, cpu, features, opt, relocModel);

  // configure module
  module->setDataLayout(targetMachine->createDataLayout());
  module->setTargetTriple(targetTriple);

  // emit object code
  auto filename = "output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(filename, EC, llvm::sys::fs::OF_None);

  if (EC) {
    throw std::runtime_error(
        fmt::format("Could not open file {}: {}", filename, EC.message()));
  }

  // define pass
  llvm::legacy::PassManager pass;
  auto fileType = llvm::CGFT_ObjectFile;

  if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
    throw std::runtime_error("targetMachine can't emit a file of this type");
  }

  pass.run(*module);
  dest.flush();

  // TA-DA

  fmt::print("Output written to {}\n", filename);
}

void CodeGenerator::dumpIR(ModuleWrapper &module) {
  module->print(llvm::errs(), nullptr);
}

} // namespace Cougar::LlvmCodeGenerator