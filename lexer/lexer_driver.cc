#include <fmt/core.h>

#include "lexer.hh"
#include "token.hh"

#include "utils/file_loader.hh"
#include "utils/zone_allocator.hh"

int main(int argc, char **argv) {
  fmt::print("Lexer!\n");

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  std::string path = argv[1];
  fmt::print("Lexing file '{}'\n", path);

  using namespace Cougar;

  Utils::FileLoader loader;
  loader.load(path);

  fmt::print("Loaded {} bytes\n", loader.getSize());

  Utils::ZoneAllocator zone;

  std::byte *buffer = zone.allocateBlock(loader.getSize());
  loader.copyTo(buffer);

  std::string_view sv((const char *)buffer, loader.getSize());

  auto tokens = Lexer::lexBuffer(sv, zone);

  for (Lexer::Token *token : tokens) {
    fmt::print("{}\n", *token);
  }
}
