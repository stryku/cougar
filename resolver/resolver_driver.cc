#include <fmt/core.h>

#include "parser/parser.hh"

#include "resolver.hh"

#include "lexer/lexer.hh"
#include "lexer/token.hh"

#include "ast/module.hh"

#include "utils/file_loader.hh"
#include "utils/zone_allocator.hh"

int main(int argc, char **argv) {
  fmt::print("Resolver!\n");

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  std::string path = argv[1];
  fmt::print("Resolving file '{}'...\n", path);

  using namespace Cougar;

  Utils::ZoneAllocator zone;

  auto tokens = Lexer::lexFile(path);

  Utils::Diagnostics diag;
  diag.setPath(path);

  Parser::Parser parser(diag);

  auto module = parser.parseModule(tokens);

  Resolver::Resolver resolver(diag);

  if (module)
    resolver.resolveModule(module);

  diag.print();

  if (module) {
    module->dump();
  }
}