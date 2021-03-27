#pragma once

#include "lexer/lexer.hh"

#include <fmt/core.h>

#include <memory>

namespace Cougar {

namespace ParserAST {
class INode;
class Module;
enum class Access;
} // namespace ParserAST

namespace Parser {

class Parser {
public:
  std::unique_ptr<ParserAST::Module> parseModule(Lexer::InputSource &input);

private:
  std::unique_ptr<ParserAST::INode> parseFunction(ParserAST::Access access);

  [[noreturn]] void throwParseError(const std::string &msg);

  template <typename... Ts>
  [[noreturn]] void throwParseError(const std::string &format,
                                    const Ts &...args) {
    throwParseError(fmt::format(format, args...));
  }

  std::unique_ptr<Lexer::Lexer> mLexer;
};

} // namespace Parser
} // namespace Cougar