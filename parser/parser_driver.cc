#include <fmt/core.h>

#include "parser.hh"

#include "lexer/lexer.hh"
#include "lexer/token.hh"

#include "ast/module.hh"

#include "utils/file_loader.hh"
#include "utils/zone_allocator.hh"

int main(int argc, char **argv) {
  fmt::print("Parser!\n");

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  using namespace Cougar;

  std::string path = argv[1];
  fmt::print("Parsing file '{}'...\n", path);

  using namespace Cougar;

  Utils::FileLoader loader;
  loader.load(path);

  fmt::print("Loaded {} bytes\n", loader.getSize());

  Utils::ZoneAllocator zone;

  std::byte *buffer = zone.allocateBlock(loader.getSize());
  loader.copyTo(buffer);

  std::string_view sv((const char *)buffer, loader.getSize());

  auto tokens = Lexer::lexBuffer(sv);

  Utils::Diagnostics diag;
  diag.setPath(path);

  Parser::Parser parser(diag);

  auto module = parser.parseModule(tokens);

  diag.print();

  if (!diag.hasErrors()) {
    module->dump();
  }
}
