#pragma once

#include "source_location.hh"

// using bison requires lexer to depend onm parser's tokens
#include "../parser/generated/Parser_generatedbase.h"

#include <fmt/format.h>

#include <string_view>

namespace Cougar::Lexer {

// oh, c++ preprocessor, you Turing-complete clusterfuck...
#define TOKEN(x) x = ::Cougar_generated::Parser_generatedBase::Tokens_::x
enum class TokenType {
#include "token_type.def"
};
#undef TOKEN

#define TOKEN(x) #x
constexpr std::string_view TOKEN_TYPE_NAMES[] = {
#include "token_type.def"
};
#undef TOKEN

class Token {
public:
  TokenType type;
  SourceLocation location;
  std::string_view content; // valid only for some
};

} // namespace Cougar::Lexer

// TokenType formatter
template <> struct fmt::formatter<Cougar::Lexer::TokenType> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Lexer::TokenType &token, FormatContext &ctx) {
    return format_to(ctx.out(), "{}",
                     Cougar::Lexer::TOKEN_TYPE_NAMES[int(token)]);
  }
};

// Token fomratter
template <> struct fmt::formatter<Cougar::Lexer::Token> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Lexer::Token &token, FormatContext &ctx) {
    return format_to(ctx.out(), "Token({} {} '{}')", token.location, token.type,
                     token.content);
  }
};