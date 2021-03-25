#pragma once

#include "type.hh"

#include <memory>
#include <unordered_map>

namespace Cougar {

class TypeContainer {
public:
  TypeContainer();
  const Type *findType(std::string_view name) const;

private:
  std::unordered_map<std::string, std::unique_ptr<Type>> mTypes;
};

} // namespace Cougar