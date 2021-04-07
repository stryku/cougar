#include "lexer.hh"

#include "token.hh"

#include "utils/utf8_decoder.hh"
#include "utils/zone_allocator.hh"

#include <algorithm>
#include <cctype>
#include <stdexcept>

#include <stdio.h>

namespace Cougar::Lexer {

namespace {

struct SingleCharToken {
  char chr;
  TokenType token;
};

constexpr SingleCharToken SINGLE_CHAR_TOKENS[] = {
    {';', TokenType::Semicolon},  {',', TokenType::Comma},
    {'{', TokenType::BraceOpen},  {'}', TokenType::BraceClose},
    {'(', TokenType::ParentOpen}, {')', TokenType::ParentClose},
    {'<', TokenType::Operator},   {'>', TokenType::Operator},

};

struct ReservedWord {
  const char *str;
  TokenType token;
};

constexpr ReservedWord RESERVED_WORDS[] = {
    {"return", TokenType::KwReturn},
    {"function", TokenType::KwFunction},
    {"private", TokenType::KwPrivate},
    {"public", TokenType::KwPublic},

};

bool isNumber(int c) { return std::isdigit(c); }

bool isIdentifierFirst(int c) { return std::isalpha(c) || c == '_'; }
bool isIdentifier(char c) { return std::isalnum(c) || c == '_'; }

class Lexer {
public:
  Lexer(std::string_view buffer, Utils::ZoneAllocator &zone)
      : mDecoder(buffer), mZone(zone) {}

  std::vector<Token *> lex();

private:
  TokenType getNext();

  Utils::rune_t readNextChar();
  void skipWhitespace();
  TokenType parseNumber();
  TokenType parseIdentifier();
  TokenType parseSingleCharacterToken();
  char mLast = 0;

  std::string mCurrentToken;
  SourceLocation mLocation;

  Utils::Utf8Decoder mDecoder;
  Utils::ZoneAllocator &mZone;
};

std::vector<Token *> Lexer::lex() {

  std::vector<Token *> out;
  TokenType type;
  do {
    type = getNext();
    std::string_view content = mZone.strdup(mCurrentToken);

    Token *token = mZone.make<Token>(Token{type, mLocation, content});
    out.push_back(token);
  } while (type != TokenType::Eof);

  return out;
}

Utils::rune_t Lexer::readNextChar() { return mDecoder.next(); }

TokenType Lexer::getNext() {
  mCurrentToken.clear();
  if (mLast == 0)
    mLast = readNextChar();

  skipWhitespace();

  if (mLast == 0) {
    return TokenType::Eof;
  }

  if (isNumber(mLast))
    return parseNumber();

  if (isIdentifierFirst(mLast))
    return parseIdentifier();

  // single characters
  return parseSingleCharacterToken();
}

TokenType Lexer::parseSingleCharacterToken() {

  auto it =
      std::find_if(std::begin(SINGLE_CHAR_TOKENS), std::end(SINGLE_CHAR_TOKENS),
                   [&](auto &sct) { return sct.chr == mLast; });
  if (it == std::end(SINGLE_CHAR_TOKENS)) {
    throw std::runtime_error(fmt::format(
        "{}: Lexer error: unknown character '{}'", mLocation, mLast));
  }

  mCurrentToken.push_back(mLast);
  mLast = readNextChar();
  mLocation.column++;
  return it->token;
}

void Lexer::skipWhitespace() {
  while (true) {
    if (mLast == ' ' || mLast == '\t') {
      mLocation.column++;
      mLast = readNextChar();
    } else if (mLast == '\n') {
      mLocation.column = 0;
      mLocation.line++;
      mLast = readNextChar();
    } else {
      return;
    }
  }
}

TokenType Lexer::parseNumber() {
  while (isNumber(mLast)) {
    mCurrentToken.push_back(char(mLast));
    mLocation.column++;
    mLast = readNextChar();
  }
  return TokenType::LitNumber;
}

TokenType Lexer::parseIdentifier() {
  while (isIdentifier(mLast)) {
    mCurrentToken.push_back(char(mLast));
    mLocation.column++;
    mLast = readNextChar();
  }

  // identify reserved words
  auto it = std::find_if(std::begin(RESERVED_WORDS), std::end(RESERVED_WORDS),
                         [&](auto &rw) { return rw.str == mCurrentToken; });
  if (it == std::end(RESERVED_WORDS))
    return TokenType::Identifier;
  else
    return it->token;
}

} // namespace

std::vector<Token *> lexBuffer(std::string_view buffer,
                               Utils::ZoneAllocator &zone) {
  Lexer lexer(buffer, zone);
  return lexer.lex();
}

} // namespace Cougar::Lexer
