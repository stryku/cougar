#pragma once

#include "utils/list.hh"
#include "utils/source_location.hh"
#include "utils/utf8_decoder.hh"

#include <string>
#include <vector>

namespace Cougar::Utils {
class ZoneAllocator;
}

namespace Cougar::Lexer {

class Token;
enum class TokenType;

class Lexer {
public:
  Lexer(std::string_view buffer) : mDecoder(buffer) {}

  Utils::List<Token> lex();

private:
  TokenType getNext();

  void readNextChar();
  void skipWhitespace();
  TokenType parseNumber();
  TokenType parseIdentifier();
  TokenType parseSingleCharacterToken();
  char mLast = 0;

  const char *mCurrentTokenBegin = nullptr;
  const char *mLastPosition = nullptr;
  Utils::SourceLocation mTokenBeginLocation;
  Utils::SourceLocation mLocation;

  Utils::Utf8Decoder mDecoder;
};

// Processes a input file, places token in memory zone,
// returns vector of pointers to tokens.
// Throws on error
Utils::List<Token> lexBuffer(std::string_view buffer);

} // namespace Cougar::Lexer
