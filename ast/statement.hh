#pragma once

#include "node.hh"

namespace Cougar::Ast {

class ParamPack;

class Statement : public NodeOnToken {
public:
  Statement(const Lexer::Token *tok = nullptr) : NodeOnToken(tok) {}
};

class StatementGroup : public Statement {
public:
  void addStatement(Statement *stm) { mStatements.emplace_back(stm); }

  Utils::List<Statement *> &statements() { return mStatements; }

private:
  void doDump(int indent = 0) const override;

  Utils::List<Statement *> mStatements;
};

class FunctioncCallStatement : public Statement {
public:
  FunctioncCallStatement(std::string_view funName, ParamPack *params,
                         const Lexer::Token *tok = nullptr)
      : Statement(tok), mFunName(funName), mParams(params) {}

private:
  void doDump(int indent = 0) const override;
  std::string_view mFunName;
  ParamPack *mParams = nullptr;
};

}; // namespace Cougar::Ast