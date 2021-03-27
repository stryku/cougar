#pragma once

#include <fmt/format.h>

#include <string>
#include <string_view>

namespace Cougar::Lexer {

enum class Token;

class InputSource;

struct SourceLocation {
  int line = 0;
  int column = 0;
};

class Lexer {
public:
  Lexer(InputSource &source);

  Token getNext();
  std::string_view getCurrentTokenText() const { return mCurrentToken; }
  SourceLocation getCurrentLocation() const { return mLocation; }

private:
  int readNextChar();
  void skipWhitespace();
  Token parseNumber();
  Token parseIdentifier();
  Token parseSingleCharacterToken();
  char mLast = 0;

  InputSource &mSource;
  std::string mCurrentToken;
  SourceLocation mLocation;
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
