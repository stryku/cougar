#include "parser.hh"

#include "ast/module.hh"

namespace Cougar::Parser {

using namespace Ast;
using namespace Utils;

using Lexer::Token;
using Lexer::TokenType;

Parser::Parser(Utils::ZoneAllocator &zone, Diagnostics &diag)
    : mZone(zone), mDiag(diag) {}

Module *Parser::parseModule(const List<Lexer::Token> &tokens) {

  Module *mod = mZone.make<Module>();

  auto tokenIt = tokens.begin();

  while (tokenIt->type != TokenType::Eof) {
    if (!(parseModuleDeclaration(mod, tokenIt))) {

      mDiag.error(tokenIt->location, "Parse error");
      return mod;
    }
  }

  return mod;
}

bool Parser::parseModuleDeclaration(Ast::Module *mod, TokenIterator &it) {
  // TODO
  (void)it;
  (void)mod;
  return false;
}

} // namespace Cougar::Parser