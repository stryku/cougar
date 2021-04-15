#pragma once

#include "utils/list.hh"
#include "utils/zone_allocator.hh"

#include "lexer/token.hh"

namespace Cougar::Ast {
class Module;
}

namespace Cougar::Parser {

class Parser {
public:
  Parser(Utils::ZoneAllocator &zone);

  Ast::Module *parseModule(const Utils::List<Lexer::Token> &tokens);

private:
  Utils::ZoneAllocator &mZone;
};

} // namespace Cougar::Parser