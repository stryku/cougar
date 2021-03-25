#pragma once

#include <fmt/format.h>

#include <string_view>

namespace Cougar {

enum class Token {
  // keywords
  KwPrivate,
  KwPublic,
  KwExported,
  KwFunction,
  KwReturn,

  // litereals
  LitNumber,
  LitString,

  // special
  BraceOpen,
  BraceClose,
  ParentOpen,
  ParentClose,
  Semicolon,
  Operator,
  Comma,

  // other
  Identifier,

  // EOF
  Eof
};

constexpr std::string_view TOKEN_NAMES[] = {
    // keywords
    "KwPrivate", "KwPublic", "KwExported", "KwFunction", "KwReturn",

    // litereals
    "LitNumber", "LitString",

    // braces, parenth, operators
    "BraceOpen", "BraceClose", "ParentOpen", "ParentClose", "Semicolon",
    "Operator", "comma",

    // other
    "Identifier",

    // eof
    "Eof"};

static_assert(TOKEN_NAMES[int(Token::Eof)] == "Eof");

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