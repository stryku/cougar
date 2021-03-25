#pragma once

#include <string>
#include <string_view>

namespace Cougar {

class Type {
public:
  Type(std::string_view name) : mName(name) {}

  const std::string &name() const { return mName; }

private:
  std::string mName;
};

} // namespace Cougar