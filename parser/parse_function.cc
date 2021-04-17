#include "parser.hh"

#include "ast/function.hh"

namespace Cougar::Parser {

using namespace Ast;
using namespace Utils;

using Lexer::Token;
using Lexer::TokenType;

Ast::FunctionDeclaration *Parser::parseFunction(TokenIterator &it,
                                                Ast::Access access) {

  // return type
  if (it->type != TokenType::Identifier) {
    mDiag.error(it->location, "Expected function return type");
    return nullptr;
  }
  const Token &typeToken = *it;
  ++it;

  // function name
  if (it->type != TokenType::Identifier) {
    mDiag.error(it->location, "Expected function name");
    return nullptr;
  }

  TypeName *t = Zone::make<TypeName>(typeToken.content, &typeToken);
  FunctionDeclaration *fun =
      Zone::make<FunctionDeclaration>(access, it->content, t, &*it);
  ++it;

  // args
  if (it->type != TokenType::ParentOpen) {
    mDiag.error(it->location, "Expected '(' here");
    return nullptr;
  }
  ++it;

  while (true) {

    // arg type
    if (it->type != TokenType::Identifier) {
      mDiag.error(it->location, "Expected argument type");
      return nullptr;
    }
    TypeName *argType = Zone::make<TypeName>(it->content, &*it);
    std::string_view argName;
    ++it;

    // maybe arg name
    if (it->type == TokenType::Identifier) {
      argName = it->content;
      ++it;
    }

    fun->addArg(argType, argName);

    // comma or closing parent
    if (it->type == TokenType::Comma) {
      ++it;
      continue;
    }

    if (it->type == TokenType::ParentClose) {
      ++it;
      break;
    }

    mDiag.error(it->location, "Expected ',' or ')' here");
  }

  // not - see if this is function defintion or declaration
  // TODO
  if (it->type != TokenType::Semicolon) {
    mDiag.error(it->location, "Function defintions not impelemented yet :(");
    return nullptr;
  }
  ++it;
  return fun;
}

} // namespace Cougar::Parser