#pragma once

#include "node.hh"

namespace Cougar::Ast {

class Expression : public NodeOnToken {
public:
  Expression(const Lexer::Token *tok = nullptr) : NodeOnToken(tok) {}
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

class StringLiteral : public Expression {
public:
  StringLiteral(std::string_view content, const Lexer::Token *tok = nullptr)
      : Expression(tok), mContent(content) {}

  std::string_view content() const { return mContent; }

private:
  void doDump(int indent = 0) const override;
  std::string_view mContent;
};

} // namespace Cougar::Ast