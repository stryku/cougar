#pragma once

#include <cstdint>

namespace Cougar::Meta {

// A cookie with opaque data for use by code generator
union Cookie {
  void *ptr = nullptr;
  std::uint64_t u64;
};

} // namespace Cougar::Meta