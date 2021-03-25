#include "lexer.hh"
#include "token.hh"

#include <algorithm>
#include <cctype>
#include <stdexcept>

#include <stdio.h>

namespace Cougar {

namespace {

bool isNumber(int c) { return std::isdigit(c); }

bool isIdentifierFirst(int c) { return std::isalpha(c) || c == '_'; }
bool isIdentifier(char c) { return std::isalnum(c) || c == '_'; }

} // namespace

int Lexer::readNextChar() { return ::getchar(); }

Token Lexer::getNext() {
  mCurrentToken.clear();
  if (mLast == 0)
    mLast = readNextChar();

  skipWhitespace();

  if (mLast < 0) {
    return Token::eof;
  }

  if (isNumber(mLast))
    return parseNumber();

  if (isIdentifierFirst(mLast))
    return parseIdentifier();

  // single characters
  return parseSingleCharacterToken();
}

Token Lexer::parseSingleCharacterToken() {
  struct SingleCharToken {
    char chr;
    Token token;
  };
  constexpr SingleCharToken SINGLE_CHAR_TOKENS[] = {
      {';', Token::semicolon},   {',', Token::comma},
      {'{', Token::brace_open},  {'}', Token::brace_close},
      {'(', Token::parent_open}, {')', Token::parent_close},
      {'<', Token::operat},      {'>', Token::operat},

  };

  auto it =
      std::find_if(std::begin(SINGLE_CHAR_TOKENS), std::end(SINGLE_CHAR_TOKENS),
                   [&](auto &sct) { return sct.chr == mLast; });
  if (it == std::end(SINGLE_CHAR_TOKENS)) {
    throw std::runtime_error(
        fmt::format("Lexer error at {}:{}. Unknown character '{}'",
                    mLocation.line, mLocation.column, mLast));
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
  return Token::lit_number;
}

Token Lexer::parseIdentifier() {
  while (isIdentifier(mLast)) {
    mCurrentToken.push_back(char(mLast));
    mLocation.column++;
    mLast = readNextChar();
  }
  return Token::identifier;
}

} // namespace Cougar
