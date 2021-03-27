#include "parser.hh"

#include "parser_ast/parser_ast.hh"

#include "lexer/token.hh"

namespace Cougar::Parser {

using Lexer::Token;
using namespace ParserAST;

std::unique_ptr<ParserAST::Module>
Parser::parseModule(Lexer::InputSource &src) {

  mLexer = std::make_unique<Lexer::Lexer>(src);

  auto module = std::make_unique<Module>();

  auto token = mLexer->getNext();
  std::optional<Token> previousToken;

  while (token != Token::Eof) {

    if (token == Token::KwFunction) {
      auto access = Access::Private;
      if (previousToken) {
        if (*previousToken == Token::KwPublic)
          access = Access::Public;
      }
      previousToken.reset();
      module->addNode(parseFunction(access));
    } else if (token == Token::KwPublic) {
      previousToken = token;
    } else if (token == Token::KwPrivate) {
      previousToken = token;
    } else {
      throwParseError("Unexpected token '{}' when parsing module",
                      mLexer->getCurrentTokenText());
    }
    token = mLexer->getNext();
  }

  mLexer.reset();
  return module;
}

void Parser::throwParseError(const std::string &msg) {
  auto loc = mLexer->getCurrentLocation();
  throw std::runtime_error(
      fmt::format("{}:{}: parse error: {}", loc.line, loc.column, msg));
}

std::unique_ptr<INode> Parser::parseFunction(Access access) {
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

  std::string name;
  std::string retType;

  // return type
  {
    auto tok = mLexer->getNext();
    if (tok != Token::Identifier) {
      throwParseError("Expected return type, got '{}'",
                      mLexer->getCurrentTokenText());
    }
    retType = mLexer->getCurrentTokenText();
  }

  // function name
  {
    auto tok = mLexer->getNext();
    if (tok != Token::Identifier) {
      throwParseError("Expected function name, got '{}'",
                      mLexer->getCurrentTokenText());
    }
    name = mLexer->getCurrentTokenText();
  }

  // TODO: eat all tokens until closing bracket for now
  {
    auto tok = mLexer->getNext();
    while (tok != Token::BraceClose)
      tok = mLexer->getNext();
  }

  return std::make_unique<FunctionDefinition>(access, retType, name);
}

} // namespace Cougar::Parser