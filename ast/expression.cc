#include "expression.hh"

namespace Cougar::Ast {

void StringLiteral::doDump(int indent) const {
  iprint(indent, "StringLiteral(\"{}\")", mContent);
}

} // namespace Cougar::Ast
