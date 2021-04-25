#pragma once

#include <string_view>

namespace Cougar::Meta {

class Scope;

class FunctionInfo {
public:
private:
  std::string_view mName;
  Scope *mScope = nullptr;
  // TODO other important fields
};

} // namespace Cougar::Meta
