#pragma once

#include "lexer/lexer.hh"

#include "types/type_container.hh"

#include <fmt/core.h>

#include <memory>

namespace Cougar {

namespace Ast {
class INode;
class Module;
enum class Access;
} // namespace Ast

class Parser {
public:
  std::unique_ptr<Ast::Module> parseModule();

private:
  std::unique_ptr<Ast::INode> parseFunction(Ast::Access access);

  [[noreturn]] void throwParseError(const std::string &msg);

  template <typename... Ts>
  [[noreturn]] void throwParseError(const std::string &format,
                                    const Ts &...args) {
    throwParseError(fmt::format(format, args...));
  }

  Lexer mLexer;
  TypeContainer mTypes;
};

} // namespace Cougar