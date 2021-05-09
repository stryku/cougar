#include "llvm_codegen/code_generator.hh"

#include "parser/parser.hh"

#include "resolver/resolver.hh"

#include "lexer/lexer.hh"
#include "lexer/token.hh"

#include "ast/module.hh"

#include "meta/built_in.hh"
#include "meta/scope.hh"

#include "utils/zone_allocator.hh"

#include <fmt/core.h>

#include <stdexcept>
#include <string_view>
#include <vector>

namespace {
enum class Phase { Lex, Parse, Resolve, Codegen, Compile, Link };

struct Args {
  Phase stopAfter = Phase::Link;
  std::vector<std::string_view> inputFiles;
};

void setStopAfter(Args &a, Phase p) {
  if (a.stopAfter != Phase::Link) {
    throw std::runtime_error(
        "Only one of: --lex, --parse, --resolve, --ir, -c allowed");
  }
  a.stopAfter = p;
}

void printHelpAndExit() {
  fmt::print("Usage: cougar [options] file...\n");
  fmt::print("Options:\n");
  fmt::print(" -h, --help  Print this message\n");
  fmt::print(" --lex       Stop after lexing\n");
  fmt::print(" --parse     Stop after parsing\n");
  fmt::print(" --resolve   Stop after resolving\n");
  fmt::print(" --ir        Stop after IR generation\n");
  fmt::print(" -c          Stop after compiling\n");

  std::exit(0);
}

Args parseArgs(int argc, char **argv) {
  Args out;

  // skip the first one
  argc--;
  argv++;

  while (argc > 0) {
    std::string_view a(*argv);
    if (a == "--lex") {
      setStopAfter(out, Phase::Lex);
    } else if (a == "--parse") {
      setStopAfter(out, Phase::Parse);
    } else if (a == "--resolve") {
      setStopAfter(out, Phase::Resolve);
    } else if (a == "--ir") {
      setStopAfter(out, Phase::Codegen);
    } else if (a == "-c") {
      setStopAfter(out, Phase::Compile);
    } else if (a == "--help" || a == "-h") {
      printHelpAndExit();
    } else if (a[0] == '-') {
      fmt::print("Unknown command line option '{}'\n", a);
      std::exit(1);
    } else {
      out.inputFiles.push_back(a);
    }

    --argc;
    ++argv;
  }
  return out;
}

void compileFile(std::string_view path, Phase stopAfter,
                 Cougar::Meta::Scope *builtIn) {

  using namespace Cougar;

  fmt::print("Compiling file '{}' ...\n", path);

  Utils::Diagnostics diag;
  diag.setPath(path);

  // Lex
  auto tokens = Lexer::lexFile(path);

  if (stopAfter == Phase::Lex) {
    fmt::print("Tokens:\n");
    for (const Lexer::Token &token : tokens) {
      fmt::print("{}\n", token);
    }
    return;
  }

  // Parse
  Parser::Parser parser(diag);
  auto module = parser.parseModule(tokens);

  if (stopAfter == Phase::Parse) {
    diag.print();
    fmt::print("Tokens:\n");
    for (const Lexer::Token &token : tokens) {
      fmt::print("{}\n", token);
    }
    fmt::print("\n");
    if (module) {
      fmt::print("AST:\n");
      module->dump();
    } else {
      fmt::print("AST not generated due to  parse errors\n");
    }
  }

  if (!module || diag.hasErrors()) {
    diag.print();
    return;
  }

  // resolve
  Resolver::CompilationState compilationState;
  compilationState.mBuildInScope = builtIn;
  compilationState.mRootScope = Utils::Zone::make<Meta::Scope>("", nullptr);
  Resolver::Resolver resolver(diag, compilationState);
  resolver.resolveModule(module);

  if (stopAfter == Phase::Resolve) {
    diag.print();

    fmt::print("Tokens:\n");
    for (const Lexer::Token &token : tokens) {
      fmt::print("{}\n", token);
    }
    fmt::print("\n");
    fmt::print("AST:\n");
    module->dump();
    fmt::print("\n");

    fmt::print("Built-in scope:\n");
    compilationState.mBuildInScope->dump();
    fmt::print("\n");

    fmt::print("Compiled scope:\n");
    compilationState.mRootScope->dump();

    return;
  }

  // codegen
  LlvmCodeGenerator::CodeGenerator codeGen;
  auto llvmModule = codeGen.generate(*module);

  if (stopAfter == Phase::Codegen) {
    diag.print();
    codeGen.dumpIR(llvmModule);
    return;
  }

  // verify
  bool moduleOk = codeGen.verify(llvmModule);
  if (!moduleOk) {
    fmt::print("Module verification failed\n");
    throw std::runtime_error("Generated IR verification failed");
  }

  codeGen.compile(llvmModule);

  // TODO link
}

} // namespace

int main(int argc, char **argv) {

  try {
    Args args = parseArgs(argc, argv);

    if (args.inputFiles.empty()) {
      throw std::runtime_error("No input files");
    }

    Cougar::Utils::ZoneAllocator commonZone;
    Cougar::Meta::Scope *builtIn = Cougar::Meta::createBuiltInScope();

    for (auto f : args.inputFiles) {
      compileFile(f, args.stopAfter, builtIn);
    }

  } catch (const std::exception &e) {
    fmt::print("Fatal error: {}\n", e.what());
    return 1;
  }
}
