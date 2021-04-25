#pragma once

#include <string_view>

namespace Cougar::Meta {

class Scope {
public:
private:
  std::string_view mName;
  Scope *mParent = nullptr;
  bool mIsBuiltIn = false;
  // TODO other important fields
};

} // namespace Cougar::Meta
