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
    {"return", TokenType::KwReturn},   {"function", TokenType::KwFunction},
    {"private", TokenType::KwPrivate}, {"public", TokenType::KwPublic},
    {"module", TokenType::KwModule},

};

bool isNumber(int c) { return std::isdigit(c); }

bool isIdentifierFirst(int c) { return std::isalpha(c) || c == '_'; }
bool isIdentifier(char c) { return std::isalnum(c) || c == '_'; }

} // namespace

Utils::List<Token> Lexer::lex() {

  mLocation.column = 1;
  mLocation.line = 1;

  Utils::List<Token> tokens;
  TokenType type;
  do {
    type = getNext();
    std::string_view content = std::string_view(
        mCurrentTokenBegin, mLastPosition - mCurrentTokenBegin);

    tokens.emplace_back(mZone, Token{type, mTokenBeginLocation, content});
  } while (type != TokenType::Eof);

  return tokens;
}

void Lexer::readNextChar() {
  mLastPosition = mDecoder.getCurrentPosition();
  mLast = mDecoder.next();
}

TokenType Lexer::getNext() {
  if (mLast == 0) {
    readNextChar();
  }

  skipWhitespace();

  mTokenBeginLocation = mLocation;
  mCurrentTokenBegin = mLastPosition;

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

  readNextChar();
  mLocation.column++;
  return it->token;
}

void Lexer::skipWhitespace() {
  while (true) {
    if (mLast == ' ' || mLast == '\t') {
      mLocation.column++;
      readNextChar();
    } else if (mLast == '\n') {
      mLocation.column = 1;
      mLocation.line++;
      readNextChar();
    } else {
      return;
    }
  }
}

TokenType Lexer::parseNumber() {
  while (isNumber(mLast)) {
    mLocation.column++;
    readNextChar();
  }
  return TokenType::LitNumber;
}

TokenType Lexer::parseIdentifier() {
  while (isIdentifier(mLast)) {
    mLocation.column++;
    readNextChar();
  }

  // identify reserved words
  std::string_view currentToken(mCurrentTokenBegin,
                                mLastPosition - mCurrentTokenBegin);
  auto it = std::find_if(std::begin(RESERVED_WORDS), std::end(RESERVED_WORDS),
                         [&](auto &rw) { return rw.str == currentToken; });
  if (it == std::end(RESERVED_WORDS))
    return TokenType::Identifier;
  else
    return it->token;
}

Utils::List<Token> lexBuffer(std::string_view buffer,
                             Utils::ZoneAllocator &zone) {
  Lexer lexer(buffer, zone);
  return lexer.lex();
}

} // namespace Cougar::Lexer
