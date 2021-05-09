#pragma once

#include "node.hh"

#include <variant>

namespace Cougar::Meta {
class FunctionInfo;
class Scope;
} // namespace Cougar::Meta

namespace Cougar::Ast {

class ParamPack;
class Statement;
class Expression;

struct StGroup {
  Utils::List<Statement *> statements;
  Meta::Scope *scope = nullptr;
};

struct StFunctionCall {
  std::string_view name;
  ParamPack *params = nullptr;
  Meta::FunctionInfo *info = nullptr;
};

struct StReturn {
  Expression *expression = nullptr;
};

class Statement : public NodeOnToken {
public:
  Statement(StGroup d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  Statement(StFunctionCall d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  Statement(StReturn d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

private:
  void doDump(int indent = 0) const override;
  std::variant<StGroup, StFunctionCall, StReturn> mData;
};

}; // namespace Cougar::Ast