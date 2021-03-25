#include "parser.hh"

#include "ast/ast.hh"

#include "lexer/token.hh"

namespace Cougar {

std::unique_ptr<Ast::Module> Parser::parseModule() {

  auto module = std::make_unique<Ast::Module>();

  auto token = mLexer.getNext();
  std::optional<Token> previousToken;

  while (token != Token::Eof) {

    if (token == Token::KwFunction) {
      auto access = Ast::Access::Private;
      if (previousToken) {
        if (*previousToken == Token::KwPublic)
          access = Ast::Access::Public;
      }
      previousToken.reset();
      module->addNode(parseFunction(access));
    } else if (token == Token::KwPublic) {
      previousToken = token;
    } else if (token == Token::KwPrivate) {
      previousToken = token;
    } else {
      throwParseError("Unexpected token '{}' when parsing module",
                      mLexer.getCurrentTokenText());
    }
    token = mLexer.getNext();
  }

  return module;
}

void Parser::throwParseError(const std::string &msg) {
  auto loc = mLexer.getCurrentLocation();
  throw std::runtime_error(
      fmt::format("{}:{}: parse error: {}", loc.line, loc.column, msg));
}

std::unique_ptr<Ast::INode> Parser::parseFunction(Ast::Access access) {
  // called just after 'function' keyword
  // expecting:
  // * return type
  // * function name
  // * (
  // * [arguments]
  // * )
  // * {
  // * [body]
  // * }

  const Type *retType = nullptr;
  std::string name;

  // return type
  {
    auto tok = mLexer.getNext();
    if (tok != Token::Identifier) {
      throwParseError("Expected return type, got '{}'",
                      mLexer.getCurrentTokenText());
    }
    retType = mTypes.findType(mLexer.getCurrentTokenText());
    if (!retType) {
      throwParseError("Unknown function return type: '{}'",
                      mLexer.getCurrentTokenText());
    }
  }

  // function name
  {
    auto tok = mLexer.getNext();
    if (tok != Token::Identifier) {
      throwParseError("Expected function name, got '{}'",
                      mLexer.getCurrentTokenText());
    }
    name = mLexer.getCurrentTokenText();
  }

  // TODO: eat all tokens until closing bracket for now
  {
    auto tok = mLexer.getNext();
    while (tok != Token::BraceClose)
      tok = mLexer.getNext();
  }

  return std::make_unique<Ast::FunctionDefinition>(access, *retType, name);
}

} // namespace Cougar