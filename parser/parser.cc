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

bool Parser::parseModuleDeclaration(Ast::Module *mod, TokenIterator &out) {

  assert(mod);

  if (out->type != TokenType::KwModule) {
    return false;
  }

  // commited
  ++out;
  if (out->type != TokenType::Identifier) {
    mDiag.error(out->location, "Expected module name");
    return true;
  }

  // check if module has a declaration already
  if (mod->declaration()) {
    mDiag.error(out->location, "Repeated module declaration");
    if (mod->declaration()->token()) {
      mDiag.error(mod->declaration()->token()->location,
                  "Originally declared here");
    }

    return true;
  }

  // modify AST
  ModuleDeclaration *decl = mZone.make<ModuleDeclaration>(out->content, &*out);
  mod->add(mZone, decl);

  // check for end of statement
  ++out;

  if (out->type != TokenType::Semicolon) {
    mDiag.error(out->location, "Expected ';' at the end of module declaration");
  }

  ++out;

  // TODO maybe eat tokens untile the next semicolon, to allow for continued
  // parsing

  return true;
}

} // namespace Cougar::Parser