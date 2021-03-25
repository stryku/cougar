#pragma once

#include <string>
#include <string_view>

namespace Cougar {

enum class Token;

struct SourceLocation {
  int line = 0;
  int column = 0;
};

class Lexer {
public:
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

  std::string mCurrentToken;
  SourceLocation mLocation;
};

} // namespace Cougar