#include <fmt/core.h>

#include "lexer.hh"
#include "token.hh"

int main(int, char **) {
  fmt::print("Lexer!\n");

  using namespace Cougar;

  Lexer lexer;

  while (true) {
    auto token = lexer.getNext();
    auto loc = lexer.getCurrentLocation();
    fmt::print("{:3}:{:3} {} : '{}'\n", loc.line, loc.column, token,
               lexer.getCurrentTokenText());
    if (token == Token::eof)
      break;
  }
}
