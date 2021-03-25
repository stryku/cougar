#include <fmt/core.h>

#include "ast/ast.hh"
#include "parser.hh"

int main(int, char **) {
  fmt::print("Parser!\n");

  using namespace Cougar;

  Parser parser;

  auto module = parser.parseModule();

  module->dump();
}
