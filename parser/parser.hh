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

  // Parsing functions
  //
  // The protocol for the parsing functions is as follows:
  // - see if the tokens under the iterator are what you are supposed to be
  // parsing
  // - if not, return false, and leave it unchanged
  // - if yes, do your best to parse itm modify the AST, leafe the iteratoir at
  // the token past the end of parsed construct
  // - throw only on really fatal errors

  bool parseModuleDeclaration(Ast::Module *mod, TokenIterator &it);

  Utils::ZoneAllocator &mZone;
  Utils::Diagnostics &mDiag;
};

} // namespace Cougar::Parser