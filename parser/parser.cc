#include "parser.hh"

#include "ast/module.hh"

namespace Cougar::Parser {

using namespace Ast;

Parser::Parser(Utils::ZoneAllocator &zone) : mZone(zone) {}

Module *Parser::parseModule(const Utils::List<Lexer::Token> &tokens) {

  Module *mod = mZone.make<Module>();

  (void)tokens;

  return mod;
}

} // namespace Cougar::Parser