#pragma once

#include <fmt/format.h>

#include <string_view>

namespace Cougar {

enum class Token {
  // keywords
  kw_private,
  kw_public,
  kw_exported,
  kw_function,
  kw_return,

  // litereals
  lit_number,
  lit_string,

  // special
  brace_open,
  brace_close,
  parent_open,
  parent_close,
  semicolon,
  operat,
  comma,

  // other
  identifier,

  // EOF
  eof
};

constexpr std::string_view TOKEN_NAMES[] = {
    // keywords
    "kw_private", "kw_public", "kw_exported", "kw_function", "kw_return",

    // litereals
    "lit_number", "lit_string",

    // braces, parenth, operators
    "brace_open", "brace_close", "parent_open", "parent_close", "semicolon",
    "operat", "comma",

    // other
    "identifier",

    // eof
    "eof"};

static_assert(TOKEN_NAMES[int(Token::eof)] == "eof");

} // namespace Cougar

template <> struct fmt::formatter<Cougar::Token> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Token &token, FormatContext &ctx) {
    return format_to(ctx.out(), "{}", Cougar::TOKEN_NAMES[int(token)]);
  }
};