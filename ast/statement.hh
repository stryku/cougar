#pragma once

#include "node.hh"

#include <variant>

namespace Cougar::Ast {

class ParamPack;

class Statement : public NodeOnToken {
public:
  struct Group {
    Utils::List<Statement *> statements;
  };

  struct FunctionCall {
    std::string_view name;
    ParamPack *params = nullptr;
  };

  Statement(Group d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  Statement(FunctionCall d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

private:
  void doDump(int indent = 0) const override;
  std::variant<Group, FunctionCall> mData;
};

}; // namespace Cougar::Ast