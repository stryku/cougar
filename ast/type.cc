#include "type.hh"

#include "meta/type_info.hh"

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

  if (mInfo) {
    iprint(indent + 2, "[resolved as:]");
    mInfo->dump(indent + 6);
  }
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