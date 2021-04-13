#pragma once

#include "source_location.hh"

#include "utils/utf8_decoder.hh"
#include "utils/zone_list.hh"

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
  Lexer(std::string_view buffer, Utils::ZoneAllocator &zone)
      : mDecoder(buffer), mZone(zone) {}

  Utils::ZoneList<Token> lex();

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
  SourceLocation mLocation;

  Utils::Utf8Decoder mDecoder;
  Utils::ZoneAllocator &mZone;
};

// Processes a input file, places token in memory zone,
// returns vector of pointers to tokens.
// Throws on error
Utils::ZoneList<Token> lexBuffer(std::string_view buffer,
                                 Utils::ZoneAllocator &zone);

} // namespace Cougar::Lexer
