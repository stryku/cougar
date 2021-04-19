#include "parser.hh"

#include "ast/statement.hh"

namespace Cougar::Parser {

using namespace Ast;
using namespace Utils;

using Lexer::Token;
using Lexer::TokenType;

Ast::Scope *Parser::parseStatements(TokenIterator &it, Scope *outer) {

  Scope *scope = Zone::make<Scope>(outer);
  TokenIterator scopeBegin = it;
  if (it->type != TokenType::BraceOpen) {
    // single statement
    Statement *s = parseStatement(it, scope);
    if (s) {
      scope->addStatement(s);
    }
    return scope;
  }

  // parse until closing bracket
  ++it;
  while (it->type != TokenType::BraceClose) {
    if (it->type == TokenType::Eof) {
      mDiag.error(it->location, "Missing closing bracket at the end of input");
      mDiag.error(scopeBegin->location, "Opening brakcet is here");
      return scope;
    }

    Statement *stmt = parseStatement(it, scope);
    if (!stmt) {
      mDiag.error(it->location, "Parse error");
      return nullptr;
    }
    scope->addStatement(stmt);
  }
  ++it;
  return scope;
}

Statement *Parser::parseStatement(TokenIterator &it, Scope *scope) {

  (void)scope;

  FunctioncCallStatement *fc = parseFunctionCall(it);

  return fc;
}

FunctioncCallStatement *Parser::parseFunctionCall(TokenIterator &it) {
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
  std::string_view funName = first->content;

  // parse param pack
  ParamPack *params = parseParamPack(it);
  if (!params) {
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

  return Zone::make<FunctioncCallStatement>(funName, params, &*first);
}

} // namespace Cougar::Parser