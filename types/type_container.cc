#include "type_container.hh"

namespace Cougar {

TypeContainer::TypeContainer() {
  // add build-in types
  mTypes["int32"] = std::make_unique<Type>("int32");
  mTypes["double"] = std::make_unique<Type>("double");
}

const Type *TypeContainer::findType(std::string_view name) const {
  auto it = mTypes.find(std::string(name));
  if (it == mTypes.end())
    return nullptr;

  return it->second.get();
}

} // namespace Cougar