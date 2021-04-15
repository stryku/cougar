#pragma once

#include <fmt/format.h>

namespace Cougar::Utils {

struct SourceLocation {
  int line = 0;
  int column = 0;
};

} // namespace Cougar::Utils

template <> struct fmt::formatter<Cougar::Utils::SourceLocation> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Utils::SourceLocation &v, FormatContext &ctx) {
    return format_to(ctx.out(), "{}:{}", v.line, v.column);
  }
};
