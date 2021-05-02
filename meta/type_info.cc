#include "type_info.hh"

#include "utils/iprint.hh"
#include "utils/overloaded.hh"

#include <cassert>
#include <stdexcept>

namespace Cougar::Meta {

using namespace Utils;

void TypeInfo::dump(int indent) const {

  std::visit(overloaded{[&](const Pointer &p) {
                          iprint(indent, "Pointer To:");
                          p.pointed->dump(indent + 2);
                        },
                        [&](const Simple &s) {
                          iprint(indent, "SimpleType({})", s.name);
                        }},
             mData);
}

void TypeInfo::setPointerType(TypeInfo *ptrType) {
  assert(ptrType);

  if (mPointerType)
    throw std::logic_error(
        "TypeInfo::setPointerType: Type already has pointer type info");

  if (!ptrType->isPointer())
    throw std::logic_error("TypeInfo::setPointerType: Not a pointer type");

  mPointerType = ptrType;
}

} // namespace Cougar::Meta