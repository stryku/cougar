#include <fmt/core.h>

#include "generated/Parser.h"

int main(int argc, char **argv) {
  fmt::print("Parser!\n");

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  (void)argv;

  Cougar_generated::Parser parser;

  parser.setDebug(Cougar_generated::Parser::DebugMode_::ON);

  parser.parse();
}
