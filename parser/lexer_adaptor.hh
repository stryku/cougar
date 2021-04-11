#pragma once

#include "utils/zone_allocator.hh"

#include "lexer/token.hh"

#include <vector>

namespace Cougar::Parser {

class LexerAdaptor {
public:
  LexerAdaptor(Utils::ZoneAllocator &zone);
  ~LexerAdaptor();

  static int lex();
  static std::string_view currentText();

  void loadFile(const std::string &s);

private:
  Utils::ZoneAllocator &mZone;
  std::vector<Lexer::Token *> mTokens;
  Lexer::Token **mCurrent = nullptr;
  Lexer::Token *mLastToken = nullptr;
};

} // namespace Cougar::Parser