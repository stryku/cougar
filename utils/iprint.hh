#pragma once

#include <fmt/core.h>

namespace Cougar::Utils {

// dump helper: prints line with indentation
template <typename... Args>
void iprint(int indent, std::string_view fmt, const Args &...args) {
  if (indent > 0)
    fmt::print("{:{}}", " ", indent);
  fmt::print(fmt, args...);
  fmt::print("\n");
}

}