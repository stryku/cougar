#pragma once

#include <fmt/format.h>

namespace Cougar::Lexer {

struct SourceLocation {
  int line = 0;
  int column = 0;
};

} // namespace Cougar::Lexer

template <> struct fmt::formatter<Cougar::Lexer::SourceLocation> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Lexer::SourceLocation &v, FormatContext &ctx) {
    return format_to(ctx.out(), ":{}:{}", v.line, v.column);
  }
};

