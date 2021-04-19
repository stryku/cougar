#include "expression.hh"

namespace Cougar::Ast {

void StringLiteral::doDump(int indent) const {
  iprint(indent, "StringLiteral(\"{}\")", mContent);
}

void ParamPack::doDump(int indent) const {
  iprint(indent, "ParamPack");
  iprint(indent + 2, "- params");
  for (const Expression *e : mParams) {
    e->dump(indent + 6);
  }
}

} // namespace Cougar::Ast
