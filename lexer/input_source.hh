#pragma once

#include <cstdio>
#include <string>

namespace Cougar::Lexer {

// Input source for Lexer
class InputSource {
public:
  InputSource(const std::string &path);
  ~InputSource();
  int readNextChar() { return std::getc(mFile); }

private:
  std::FILE *mFile;
};

} // namespace Cougar::Lexer