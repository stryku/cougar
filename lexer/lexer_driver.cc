#include <fmt/core.h>

#include "input_source.hh"
#include "lexer.hh"
#include "token.hh"

int main(int argc, char **argv) {
  fmt::print("Lexer!\n");

  using namespace Cougar::Lexer;

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  InputSource src(argv[1]);
  Lexer lexer(src);

  while (true) {
    auto token = lexer.getNext();
    auto loc = lexer.getCurrentLocation();
    fmt::print("{:3}:{:3} {} : '{}'\n", loc.line, loc.column, token,
               lexer.getCurrentTokenText());
    if (token == Token::Eof)
      break;
  }
}
