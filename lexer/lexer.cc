#include "lexer.hh"

#include "token.hh"

#include "input_source.hh"

#include <algorithm>
#include <cctype>
#include <stdexcept>

#include <stdio.h>

namespace Cougar::Lexer {

namespace {

struct SingleCharToken {
  char chr;
  Token token;
};

constexpr SingleCharToken SINGLE_CHAR_TOKENS[] = {
    {';', Token::Semicolon},  {',', Token::Comma},
    {'{', Token::BraceOpen},  {'}', Token::BraceClose},
    {'(', Token::ParentOpen}, {')', Token::ParentClose},
    {'<', Token::Operator},   {'>', Token::Operator},

};

struct ReservedWord {
  const char *str;
  Token token;
};

constexpr ReservedWord RESERVED_WORDS[] = {
    {"return", Token::KwReturn},
    {"function", Token::KwFunction},
    {"private", Token::KwPrivate},
    {"public", Token::KwPublic},

};

bool isNumber(int c) { return std::isdigit(c); }

bool isIdentifierFirst(int c) { return std::isalpha(c) || c == '_'; }
bool isIdentifier(char c) { return std::isalnum(c) || c == '_'; }

} // namespace

Lexer::Lexer(InputSource &is) : mSource(is) {}

int Lexer::readNextChar() { return mSource.readNextChar(); }

Token Lexer::getNext() {
  mCurrentToken.clear();
  if (mLast == 0)
    mLast = readNextChar();

  skipWhitespace();

  if (mLast < 0) {
    return Token::Eof;
  }

  if (isNumber(mLast))
    return parseNumber();

  if (isIdentifierFirst(mLast))
    return parseIdentifier();

  // single characters
  return parseSingleCharacterToken();
}

Token Lexer::parseSingleCharacterToken() {

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
    } else if (mLast == '\r') {
      mLast = readNextChar();
    } else {
      return;
    }
  }
}

Token Lexer::parseNumber() {
  while (isNumber(mLast)) {
    mCurrentToken.push_back(char(mLast));
    mLocation.column++;
    mLast = readNextChar();
  }
  return Token::LitNumber;
}

Token Lexer::parseIdentifier() {
  while (isIdentifier(mLast)) {
    mCurrentToken.push_back(char(mLast));
    mLocation.column++;
    mLast = readNextChar();
  }

  // identify reserved words
  auto it = std::find_if(std::begin(RESERVED_WORDS), std::end(RESERVED_WORDS),
                         [&](auto &rw) { return rw.str == mCurrentToken; });
  if (it == std::end(RESERVED_WORDS))
    return Token::Identifier;
  else
    return it->token;
}

} // namespace Cougar::Lexer
