#include "code_generator.hh"

#include "ast/expression.hh"

#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace Cougar::LlvmCodeGenerator {

llvm::Value *CodeGenerator::generateExpression(Ast::ExStringLiteral &strLit) {
  // TODO

  // llvm::ArrayType *arrayType = llvm::ArrayType::get(
  //     llvm::Type::getInt8PtrTy(*mContext), strLit.content.size() + 1);

  // llvm::GlobalVariable *strConst = new llvm::GlobalVariable(
  //     *mModule,                                        // module
  //     arrayType,                                       // type
  //     true,                                            // is const
  //     llvm::GlobalValue::LinkageTypes::PrivateLinkage, // linkage
  //     nullptr                                          // initializer
  // );

  llvm::Constant *c =
      llvm::ConstantDataArray::getString(*mContext, strLit.content);

  return c;
}

} // namespace Cougar::LlvmCodeGenerator