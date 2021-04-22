#pragma once

#include "node.hh"

namespace Cougar::Ast {

class FunctionDeclaration;

// base class for all module-level statements
class ModuleStatement : public NodeOnToken {
public:
  ModuleStatement(const Lexer::Token *token = nullptr) : NodeOnToken(token) {}
};

class ModuleDeclaration : public ModuleStatement {
public:
  ModuleDeclaration(std::string_view moduleName,
                    const Lexer::Token *token = nullptr)
      : ModuleStatement(token), mModuleName(moduleName) {}

  std::string_view moduleName() const { return mModuleName; }

private:
  void doDump(int indent = 0) const override;

  std::string_view mModuleName;
};

class ModuleFunction : public ModuleStatement {
public:
  ModuleFunction(FunctionDeclaration *fun, const Lexer::Token *token = nullptr)
      : ModuleStatement(token), mFunction(fun) {
    assert(fun);
  }

private:
  void doDump(int indent = 0) const override;

  FunctionDeclaration *mFunction = nullptr;
};

class Module : public Node {
public:
  void add(ModuleDeclaration *decl);
  void add(FunctionDeclaration *fun);

  const ModuleDeclaration *declaration() const { return mDeclaration; }

private:
  void doDump(int indent = 0) const override;

  ModuleDeclaration *mDeclaration = nullptr;
  Utils::List<ModuleStatement *> mStatements;
};

} // namespace Cougar::Ast