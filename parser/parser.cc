#include "parser.hh"

#include "ast/function.hh"
#include "ast/module.hh"

namespace Cougar::Parser {

using namespace Ast;
using namespace Utils;

using Lexer::Token;
using Lexer::TokenType;

Parser::Parser(Diagnostics &diag) : mDiag(diag) {}

Module *Parser::parseModule(const List<Lexer::Token> &tokens) {

  Module *mod = Zone::make<Module>();

  auto tokenIt = tokens.begin();

  while (tokenIt->type != TokenType::Eof) {
    auto it = parseModuleDeclaration(mod, tokenIt);
    if (it == tokenIt)
      it = parseModuleFunction(mod, tokenIt);
    if (it == tokenIt) {
      mDiag.error(tokenIt->location, "Parse error");
      return mod;
    }

    tokenIt = it;
  }

  return mod;
}

Parser::TokenIterator Parser::parseModuleDeclaration(Ast::Module *mod,
                                                     TokenIterator it) {

  assert(mod);

  if (it->type != TokenType::KwModule) {
    return it;
  }

  // commited
  ++it;
  if (it->type != TokenType::Identifier) {
    mDiag.error(it->location, "Expected module name");
    return it;
  }
  const Token *idToken = &*it;

  // check for end of statement
  ++it;

  if (it->type != TokenType::Semicolon) {
    mDiag.error(it->location, "Expected ';' at the end of module declaration");
    // TODO maybe eat tokens untile the next semicolon, to allow for continued
    // parsing
  }

  ++it;

  // check if module has a declaration already
  if (mod->declaration()) {
    mDiag.error(idToken->location, "Repeated module declaration");
    if (mod->declaration()->token()) {
      mDiag.error(mod->declaration()->token()->location,
                  "Originally declared here");
    }

    return it;
  }

  // modify AST
  ModuleDeclaration *decl =
      Zone::make<ModuleDeclaration>(idToken->content, idToken);
  mod->add(decl);

  return it;
}

Parser::TokenIterator Parser::parseModuleFunction(Ast::Module *mod,
                                                  TokenIterator it) {

  Access access = Access::Private; // default for module-level functions
  TokenIterator begin = it;

  // optional access
  if (it->type == TokenType::KwPrivate) {
    ++it;
  } else if (it->type == TokenType::KwPublic) {
    access = Access::Public;
    ++it;
  } else if (it->type == TokenType::KwExtern) {
    access = Access::External;
    ++it;
  }

  // function keyword
  if (it->type != TokenType::KwFunction)
    return begin;
  ++it;

  /// -- commited from now on --
  FunctionDeclaration *fun = parseFunction(it, access);
  if (fun)
    mod->add(fun);

  return it;
}

} // namespace Cougar::Parser