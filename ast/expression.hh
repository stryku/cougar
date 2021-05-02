#pragma once

#include "node.hh"

#include <variant>

namespace Cougar::Ast {

struct ExStringLiteral {
  std::string_view content;
};

class Expression : public NodeOnToken {
public:
  Expression(const ExStringLiteral &d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

private:
  void doDump(int indent = 0) const override;

  std::variant<ExStringLiteral> mData;
};

// pack of parameters passes to actual function call
class ParamPack : public Node {
public:
  void addParam(Expression *expr) { mParams.emplace_back(expr); }

  Utils::List<Expression *> &params() { return mParams; }
  const Utils::List<Expression *> &params() const { return mParams; }

private:
  void doDump(int indent = 0) const override;
  Utils::List<Expression *> mParams;
};

} // namespace Cougar::Ast