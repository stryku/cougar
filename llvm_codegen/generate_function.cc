#include "code_generator.hh"

#include "ast/function.hh"
#include "ast/statement.hh"
#include "ast/type.hh"

#include "meta/function_info.hh"
#include "meta/type_info.hh"

#include "utils/overloaded.hh"

#include <llvm/ADT/SmallVector.h>

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

namespace Cougar::LlvmCodeGenerator {

llvm::Type *CodeGenerator::simpleTypeToLlvm(std::string_view name) {
  if (name == "int8")
    return llvm::Type::getInt8Ty(*mContext);
  else if (name == "int16")
    return llvm::Type::getInt16Ty(*mContext);
  else if (name == "int32")
    return llvm::Type::getInt32Ty(*mContext);
  else if (name == "int64")
    return llvm::Type::getInt64Ty(*mContext);
  else if (name == "cstr")
    return llvm::Type::getInt8PtrTy(*mContext);

  throw std::logic_error(fmt::format("ICE: Unknown internal type '{}'", name));
}

llvm::Type *CodeGenerator::pointerTypeToLlvm(Meta::TypeInfo *pointed) {
  return toLlvm(pointed)->getPointerTo();
}

llvm::Type *CodeGenerator::toLlvm(Meta::TypeInfo *ti) {
  assert(ti);

  if (ti->codegenData.ptr)
    return reinterpret_cast<llvm::Type *>(ti->codegenData.ptr);

  // resolve the type
  llvm::Type *t = ti->visit(overloaded{
      [&](Meta::TypeInfo::Simple &d) { return simpleTypeToLlvm(d.name); },
      [&](Meta::TypeInfo::Pointer &d) {
        return pointerTypeToLlvm(d.pointed);
      }});

  ti->codegenData.ptr = t;
  return t;
}

void CodeGenerator::generateFunction(Ast::FunctionDeclaration &funAST,
                                     llvm::Module &module) {
  Meta::FunctionInfo *info = funAST.info();

  if (!info) {
    throw std::runtime_error(
        fmt::format("ICE: meta information for function '{}' not generated",
                    funAST.name()));
  }

  // get types
  llvm::Type *returnType = toLlvm(info->returnType);

  llvm::SmallVector<llvm::Type *, 8> argTypes;
  for (auto &argAST : funAST.args()) {
    assert(argAST.type);
    argTypes.push_back(toLlvm(argAST.type->info()));
  }

  llvm::FunctionType *functionType =
      llvm::FunctionType::get(returnType, argTypes, false);

  llvm::Function *function =
      llvm::Function::Create(functionType, llvm::Function::ExternalLinkage,
                             llvm::Twine(info->name), module);

  info->codegenData.ptr = function;

  Ast::Statement *body = funAST.body();
  if (body) {
    body->visit(overloaded{
        [&](Ast::StGroup &grp) { generateFunctionBody(function, grp); },
        [&](auto &) {
          throw std::logic_error("CodeGenerator::generateFunction: function "
                                 "body expected to be a statement group");
        }});
  }
}

void CodeGenerator::generateFunctionBody(llvm::Function *llvmFunction,
                                         Ast::StGroup &body) {

  llvm::BasicBlock *bb =
      llvm::BasicBlock::Create(*mContext, "entry", llvmFunction);

  mBuilder->SetInsertPoint(bb);

  generateStatement(body);
}

} // namespace Cougar::LlvmCodeGenerator
