#include <fmt/core.h>

#include "generated/Parser_generated.h"

#include "lexer_adaptor.hh"

int main(int argc, char **argv) {
  fmt::print("Parser!\n");

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  std::string path = argv[1];
  fmt::print("Parsing file '{}'\n", path);

  using namespace Cougar;

  Utils::ZoneAllocator zone;
  Parser::LexerAdaptor adaptor(zone);
  adaptor.loadFile(path);

  Cougar_generated::Parser_generated parser;

  parser.setDebug(Cougar_generated::Parser_generated::DebugMode_::ON);

  parser.parse();
}
