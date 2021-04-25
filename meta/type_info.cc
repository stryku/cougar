#include "type_info.hh"

#include "utils/iprint.hh"

#include <cassert>
#include <stdexcept>

namespace Cougar::Meta {

using namespace Utils;

void TypeInfo::dump(int indent) const {
  // TODO visit variant
  iprint(indent, "Type()");
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