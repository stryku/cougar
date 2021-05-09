#include "code_generator.hh"

#include "ast/expression.hh"
#include "ast/statement.hh"

#include "meta/function_info.hh"

#include "utils/overloaded.hh"

#include <llvm/IR/IRBuilder.h>

namespace Cougar::LlvmCodeGenerator {

void CodeGenerator::generateStatement(Ast::StGroup &grp) {

  for (Ast::Statement *stmt : grp.statements) {
    stmt->visit([&](auto &d) { generateStatement(d); });
  }
}

void CodeGenerator::generateStatement(Ast::StFunctionCall &stmt) {

  assert(stmt.info);

  llvm::Function *fun =
      reinterpret_cast<llvm::Function *>(stmt.info->codegenData.ptr);

  assert(fun);

  llvm::SmallVector<llvm::Value *, 8> args;

  for (Ast::Expression *param : stmt.params->params()) {

    args.push_back(
        param->visit([&](auto &e) { return generateExpression(e); }));
  }

  mBuilder->CreateCall(fun, args, llvm::Twine(stmt.name));
}

void CodeGenerator::generateStatement(Ast::StReturn &) {
  // TODO
}

} // namespace Cougar::LlvmCodeGenerator