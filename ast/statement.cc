#include "statement.hh"

#include "expression.hh"

namespace Cougar::Ast {

namespace {

void dumpStatement(int indent, const Statement::Group &g) {
  iprint(indent, "StatementGroup:");
  iprint(indent + 2, "- statements");
  for (const Statement *stm : g.statements) {
    stm->dump(indent + 6);
  }
}

void dumpStatement(int indent, const Statement::FunctionCall &f) {
  iprint(indent, "FunctionCallStmt(name={})", f.name);
  assert(f.params);
  if (!f.params->params().empty()) {
    iprint(indent + 2, "- params");
    for (const Expression *e : f.params->params()) {
      e->dump(indent + 6);
    }
  }
}

} // namespace

void Statement::doDump(int indent) const {
  std::visit([&](auto &s) { dumpStatement(indent, s); }, mData);
}

} // namespace Cougar::Ast