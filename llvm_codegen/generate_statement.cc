#include "code_generator.hh"

#include "ast/statement.hh"

#include "utils/overloaded.hh"

#include <llvm/IR/IRBuilder.h>

namespace Cougar::LlvmCodeGenerator {

void CodeGenerator::generateStatement(Ast::StGroup &grp) {

  for (Ast::Statement *stmt : grp.statements) {
    stmt->visit([&](auto &d) { generateStatement(d); });
  }
}

void CodeGenerator::generateStatement(Ast::StFunctionCall &) {
  // TODO
}

} // namespace Cougar::LlvmCodeGenerator