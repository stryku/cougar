#pragma once

#include "node.hh"

#include <variant>

namespace Cougar::Ast {

class ParamPack;
class Statement;

struct StGroup {
  Utils::List<Statement *> statements;
};

struct StFunctionCall {
  std::string_view name;
  ParamPack *params = nullptr;
};

class Statement : public NodeOnToken {
public:
  Statement(StGroup d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  Statement(StFunctionCall d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

private:
  void doDump(int indent = 0) const override;
  std::variant<StGroup, StFunctionCall> mData;
};

}; // namespace Cougar::Ast