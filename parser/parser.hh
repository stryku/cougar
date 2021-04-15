#pragma once

#include "utils/diagnostics.hh"
#include "utils/list.hh"
#include "utils/zone_allocator.hh"

#include "lexer/token.hh"

namespace Cougar::Ast {
class Module;
}

namespace Cougar::Parser {

class Parser {
public:
  Parser(Utils::ZoneAllocator &zone, Utils::Diagnostics &diag);

  Ast::Module *parseModule(const Utils::List<Lexer::Token> &tokens);

private:
  using TokenIterator = Utils::List<Lexer::Token>::const_iterator;

  // parsing functions
  bool parseModuleDeclaration(Ast::Module *mod, TokenIterator &it);

  Utils::ZoneAllocator &mZone;
  Utils::Diagnostics &mDiag;
};

} // namespace Cougar::Parser