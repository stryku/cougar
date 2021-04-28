#include "parser.hh"

#include "ast/statement.hh"

namespace Cougar::Parser {

using namespace Ast;
using namespace Utils;

using Lexer::Token;
using Lexer::TokenType;

Ast::Statement *Parser::parseStatements(TokenIterator &it) {

  TokenIterator scopeBegin = it;
  if (it->type != TokenType::BraceOpen) {
    // single statement
    Statement *s = parseStatement(it);
    return s;
  }

  // a brace-enclosed gropup then. Parse until closing bracket
  Statement::Group group;
  ++it;
  while (it->type != TokenType::BraceClose) {
    if (it->type == TokenType::Eof) {
      mDiag.error(it->location, "Missing closing bracket at the end of input");
      mDiag.error(scopeBegin->location, "Opening bracket is here");
      return nullptr;
    }

    Statement *stmt = parseStatement(it);
    if (!stmt) {
      mDiag.error(it->location, "Parse error");
      return nullptr;
    }
    group.statements.emplace_back(stmt);
  }
  ++it;
  return Zone::make<Statement>(group, &*scopeBegin);
}

Statement *Parser::parseStatement(TokenIterator &it) {

  Statement *fc = parseFunctionCall(it);

  return fc;
}

Statement *Parser::parseFunctionCall(TokenIterator &it) {
  TokenIterator first = it;
  if (it->type != TokenType::Identifier)
    return nullptr;
  ++it;

  if (it->type != TokenType::ParentOpen) {
    it = first;
    return nullptr;
  }
  ++it;

  // commited here
  Statement::FunctionCall fun;
  fun.name = first->content;

  // parse param pack
  fun.params = parseParamPack(it);
  if (!fun.params) {
    return nullptr;
  }

  if (it->type != TokenType::ParentClose) {
    mDiag.error(it->location, "Expected ')'");
    return nullptr;
  }
  ++it;

  if (it->type != TokenType::Semicolon) {
    mDiag.error(it->location, "Expected ';'");
    return nullptr;
  }
  ++it;

  return Zone::make<Statement>(fun, &*first);
}

} // namespace Cougar::Parser