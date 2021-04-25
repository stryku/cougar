#include "lexer.hh"

#include "token.hh"

#include "utils/file_loader.hh"
#include "utils/utf8_decoder.hh"
#include "utils/zone_allocator.hh"

#include <algorithm>
#include <cctype>
#include <stdexcept>

#include <stdio.h>

namespace Cougar::Lexer {

using namespace std::string_view_literals;

namespace {

struct SingleCharToken {
  Utils::rune_t chr;
  TokenType token;
};

constexpr SingleCharToken SINGLE_CHAR_TOKENS[] = {
    {';', TokenType::Semicolon},  {',', TokenType::Comma},
    {'{', TokenType::BraceOpen},  {'}', TokenType::BraceClose},
    {'(', TokenType::ParentOpen}, {')', TokenType::ParentClose},
    {'<', TokenType::Operator},   {'>', TokenType::Operator},
    {'*', TokenType::Asterisk},

};

struct ReservedWord {
  std::string_view str;
  TokenType token;
};

constexpr ReservedWord RESERVED_WORDS[] = {{"return", TokenType::KwReturn},
                                           {"function", TokenType::KwFunction},
                                           {"private", TokenType::KwPrivate},
                                           {"public", TokenType::KwPublic},
                                           {"module", TokenType::KwModule},
                                           {"extern", TokenType::KwExtern}

};

bool isNumber(Utils::rune_t c) { return std::isdigit(c); }

bool isIdentifierFirst(Utils::rune_t c) { return std::isalpha(c) || c == '_'; }
bool isIdentifier(Utils::rune_t c) { return std::isalnum(c) || c == '_'; }
bool isStringLiteralFirst(Utils::rune_t c) { return c == '"'; }
} // namespace

Utils::List<Token> Lexer::lex() {

  mLocation.column = 1;
  mLocation.line = 1;

  Utils::List<Token> tokens;
  Token tok;
  do {
    tok = getNext();
    tokens.emplace_back(tok);
  } while (tok.type != TokenType::Eof);

  return tokens;
}

// Helper. Makes token form cyurrent state
Token Lexer::makeToken(TokenType type) {
  std::string_view content =
      std::string_view(mCurrentTokenBegin, mLastPosition - mCurrentTokenBegin);
  return Token{type, mTokenBeginLocation, content};
}

void Lexer::readNextChar() {
  mLastPosition = mDecoder.getCurrentPosition();
  mLast = mDecoder.next();
}

Token Lexer::getNext() {
  if (mLast == 0) {
    readNextChar();
  }

  skipWhitespace();

  mTokenBeginLocation = mLocation;
  mCurrentTokenBegin = mLastPosition;

  if (mLast == 0) {
    return makeToken(TokenType::Eof);
  }

  if (isNumber(mLast))
    return parseNumber();

  if (isIdentifierFirst(mLast))
    return parseIdentifier();

  if (isStringLiteralFirst(mLast))
    return parseStringLiteral();

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

  readNextChar();
  mLocation.column++;
  return makeToken(it->token);
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

Token Lexer::parseNumber() {
  while (isNumber(mLast)) {
    mLocation.column++;
    readNextChar();
  }
  return makeToken(TokenType::LitNumber);
}

Token Lexer::parseIdentifier() {
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
    return makeToken(TokenType::Identifier);
  else
    return makeToken(it->token);
}

Token Lexer::parseStringLiteral() {
  readNextChar(); // skip opening quote
  // find content between the quotes
  // TODO: handle escapes
  // TODO: join consecutive literals into one
  // TODO handle raw string/heredoc
  const char *firstPos = mLastPosition;
  const char *lastPos = mLastPosition;
  while (mLast != '"') {
    if (mLast == 0)
      throw std::runtime_error(fmt::format(
          "{}: Lexer error: unerminated string literal", mLocation));
    readNextChar();
    lastPos = mLastPosition;
  }
  readNextChar(); // skip past the closing quote
  std::string_view content(firstPos, lastPos - firstPos);
  return Token{TokenType::LitString, mTokenBeginLocation, content};
}

Utils::List<Token> lexBuffer(std::string_view buffer) {
  Lexer lexer(buffer);
  return lexer.lex();
}

Utils::List<Token> lexFile(const std::string &path) {
  Utils::FileLoader loader;
  loader.load(path);
  std::byte *buffer = Utils::Zone::allocateBlock(loader.getSize());
  loader.copyTo(buffer);

  std::string_view sv((const char *)buffer, loader.getSize());

  return lexBuffer(sv);
}

} // namespace Cougar::Lexer
