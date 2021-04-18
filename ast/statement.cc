#include "statement.hh"

#include "expression.hh"

namespace Cougar::Ast {

void Scope::doDump(int indent) const {
  iprint(indent, "Scope:");
  iprint(indent + 2, "- statements");
  for (const Statement *stm : mStatements) {
    stm->dump(indent + 6);
  }
}

void FunctioncCallStatement::doDump(int indent) const {
  iprint(indent, "FunctionCallStmt(name={}", mFunName);
  if (!mParams->params().empty()) {
    iprint(indent + 2, "- params");
    for (const Expression *e : mParams->params()) {
      e->dump(indent + 6);
    }
  }
}

} // namespace Cougar::Ast