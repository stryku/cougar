#include <fmt/core.h>

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

  Utils::ZoneAllocator allocator;

  std::byte *buffer = allocator.allocateBlock(loader.getSize());
  loader.copyTo(buffer);

  // InputSource src(argv[1]);
  // Lexer lexer(src);

  // while (true) {
  //   auto token = lexer.getNext();
  //   auto loc = lexer.getCurrentLocation();
  //   fmt::print("{:3}:{:3} {} : '{}'\n", loc.line, loc.column, token,
  //              lexer.getCurrentTokenText());
  //   if (token == Token::Eof)
  //     break;
  // }
}
