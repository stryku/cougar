#include "type.hh"

#include "utils/overloaded.hh"

namespace Cougar::Ast {

void TypeNode::doDump(int indent) const {

  std::visit(overloaded{[&](const Pointer &ptr) {
                          iprint(indent, "PointerTo:");
                          ptr.mPointedType->dump(indent + 2);
                        },
                        [&](const Named &n) {
                          iprint(indent, "Type(name={})", n.mName);
                        }},
             mData);
}

std::string TypeNode::name() const {
  return std::visit(
      overloaded{[&](const Pointer &ptr) {
                   return fmt::format("{}*", ptr.mPointedType->name());
                 },
                 [&](const Named &n) { return std::string(n.mName); }},
      mData);
}

} // namespace Cougar::Ast