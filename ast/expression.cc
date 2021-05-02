#include "expression.hh"

#include "utils/overloaded.hh"

#include "meta/type_info.hh"

namespace Cougar::Ast {

void Expression::doDump(int indent) const {
  std::visit(overloaded{[&](const ExStringLiteral &d) {
               iprint(indent, "StringLiteral(\"{}\")", d.content);
             }},
             mData);
  if (mType) {
    iprint(indent + 2, "[type resolved as:]");
    mType->dump(indent + 6);
  }
}

void ParamPack::doDump(int indent) const {
  iprint(indent, "ParamPack");
  iprint(indent + 2, "- params");
  for (const Expression *e : mParams) {
    e->dump(indent + 6);
  }
}

} // namespace Cougar::Ast
