#pragma once

#include <string_view>

namespace Cougar::Meta {

class Scope;

class TypeInfo {
public:
  TypeInfo(std::string_view name) : mName(name) {}

  void dump(int indent) const;

private:
  std::string_view mName;
  Scope *mScope = nullptr;
  // TODO other important fields
};

} // namespace Cougar::Meta
