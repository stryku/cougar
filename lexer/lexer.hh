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
  Token makeToken(TokenType type);

  Token getNext();

  void readNextChar();
  void skipWhitespace();
  Utils::rune_t peekNext();

  Token parseNumber();
  Token parseIdentifier();
  Token parseSingleCharacterToken();
  Token parseStringLiteral();
  Token parseStringLiteralWithEscapedCharacters(const char *firstPos);

  Utils::rune_t readEscapedCharacter();

  bool atNumberLiteral() const;

  Utils::rune_t mLast = 0;

  const char *mCurrentTokenBegin = nullptr;
  const char *mLastPosition = nullptr;
  Utils::SourceLocation mTokenBeginLocation;
  Utils::SourceLocation mLocation;

  Utils::Utf8Decoder mDecoder;
};

// Processes a input data buffer, places token in memory zone,
// returns list of pointers to tokens (in the Zone).
// Throws on error
Utils::List<Token> lexBuffer(std::string_view buffer);

// Processes a input file, places token in memory zone,
// returns list of pointers to tokens (in the Zone).
// Throws on error
Utils::List<Token> lexFile(const std::string &path);

inline Utils::List<Token> lexFile(std::string_view path) {
  return lexFile(std::string(path));
}

} // namespace Cougar::Lexer
