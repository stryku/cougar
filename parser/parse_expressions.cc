#include "parser.hh"

#include "ast/expression.hh"

namespace Cougar::Parser {

using namespace Ast;
using namespace Utils;

using Lexer::Token;
using Lexer::TokenType;

ParamPack *Parser::parseParamPack(TokenIterator &it) {
  ParamPack *pp = Zone::make<ParamPack>();

  while (true) {
    Expression *e = parseExpression(it);
    if (!e)
      break;
    pp->addParam(e);
  }

  return pp;
}

Expression *Parser::parseExpression(TokenIterator &it) {
  if (it->type == TokenType::LitString) {
    StringLiteral *sl = Zone::make<StringLiteral>(it->content, &*it);
    ++it;
    return sl;
  }
  return nullptr;
}

} // namespace Cougar::Parser
