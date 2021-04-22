#include <fmt/core.h>

#include "lexer.hh"
#include "token.hh"

#include "utils/zone_allocator.hh"

int main(int argc, char **argv) {
  fmt::print("Lexer!\n");

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  std::string path = argv[1];
  fmt::print("Lexing file '{}'\n", path);

  using namespace Cougar;

  Utils::ZoneAllocator zone;
  auto tokens = Lexer::lexFile(path);

  for (const Lexer::Token &token : tokens) {
    fmt::print("{}\n", token);
  }
}
