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

    ExStringLiteral sl;
    sl.content = it->content;

    Expression *e = Zone::make<Expression>(sl, &*it);
    ++it;
    return e;
  }
  return nullptr;
}

} // namespace Cougar::Parser
