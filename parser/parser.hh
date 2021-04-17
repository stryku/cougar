#pragma once

#include "utils/diagnostics.hh"
#include "utils/list.hh"
#include "utils/zone_allocator.hh"

#include "lexer/token.hh"

namespace Cougar::Ast {
class Module;
class FunctionDeclaration;
enum class Access;
} // namespace Cougar::Ast

namespace Cougar::Parser {

class Parser {
public:
  Parser(Utils::Diagnostics &diag);

  Ast::Module *parseModule(const Utils::List<Lexer::Token> &tokens);

private:
  using TokenIterator = Utils::List<Lexer::Token>::const_iterator;

  // Parsing functions
  //
  // The protocol for the parsing functions is as follows:
  // - see if the tokens under the iterator are what you are supposed to be
  // parsing
  // - if not, return itertator unchanged
  // - if yes, do your best to parse it, modify the AST, return iterator past
  // the last statement consumed
  // - throw only on really fatal errors

  // parse 'module MyModule;'
  TokenIterator parseModuleDeclaration(Ast::Module *mod, TokenIterator it);

  // parse module-level function declaration or definition
  TokenIterator parseModuleFunction(Ast::Module *mod, TokenIterator it);

  // Parse helpers
  //
  // Employed once commited.
  // Protocol: return AST object; move iterator past the end

  Ast::FunctionDeclaration *parseFunction(TokenIterator &it,
                                          Ast::Access access);

  Utils::Diagnostics &mDiag;
};

} // namespace Cougar::Parser