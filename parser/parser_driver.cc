#include <fmt/core.h>

#include "parser.hh"
#include "parser_ast/parser_ast.hh"

#include "lexer/input_source.hh"

int main(int argc, char **argv) {
  fmt::print("Parser!\n");

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  using namespace Cougar;

  Lexer::InputSource src(argv[1]);
  Parser::Parser parser;

  auto module = parser.parseModule(src);
  module->dump();
}
