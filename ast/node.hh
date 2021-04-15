#pragma once

#include "lexer/token.hh"

#include "utils/list.hh"

namespace Cougar::Ast {

class Node {
public:
  void dump(int indent = 0) const { doDump(indent); }

protected:
  virtual void doDump(int indent) const = 0;
};

class NodeOnToken : public Node {
public:
  NodeOnToken(const Lexer::Token *token = nullptr) : mToken(token) {}
  const Lexer::Token *token() const { return mToken; }

private:
  const Lexer::Token *mToken;
};

// dump helper: prints line with indentation
template <typename... Args>
void iprint(int indent, std::string_view fmt, const Args &...args) {
  if (indent > 0)
    fmt::print("{:{}}", " ", indent);
  fmt::print(fmt, args...);
  fmt::print("\n");
}

enum class Access { Public, Private, External };

} // namespace Cougar::Ast

template <> struct fmt::formatter<Cougar::Ast::Access> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Ast::Access &v, FormatContext &ctx) {
    using namespace Cougar::Ast;
    switch (v) {
    case Access::Public:
      return format_to(ctx.out(), "Public");
    case Access::Private:
      return format_to(ctx.out(), "Private");
    case Access::External:
      return format_to(ctx.out(), "External");
    }
    return format_to(ctx.out(), "?unknown access({})", int(v));
  }
};
