#include "type.hh"

#include "utils/overloaded.hh"

namespace Cougar::Ast {

void TypeNode::doDump(int indent) const {

  std::visit(overloaded{[&](const Pointer &ptr) {
                          iprint(indent, "PointerTo:");
                          ptr.pointedType->dump(indent + 2);
                        },
                        [&](const Named &n) {
                          iprint(indent, "Type(name={})", n.name);
                        }},
             mData);
}

std::string TypeNode::name() const {
  return std::visit(
      overloaded{[&](const Pointer &ptr) {
                   return fmt::format("{}*", ptr.pointedType->name());
                 },
                 [&](const Named &n) { return std::string(n.name); }},
      mData);
}

} // namespace Cougar::Ast