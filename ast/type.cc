#include "type.hh"

namespace Cougar::Ast {

void TypeName::doDump(int indent) const {
  iprint(indent, "TypeName(name={})", mTypeName);
}

void PointerTo::doDump(int indent) const {
  iprint(indent, "PointerTo:");
  mPointee->dump(indent + 2);
}

} // namespace Cougar::Ast