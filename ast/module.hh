#pragma once

#include "node.hh"

namespace Cougar::Ast {

class FunctionDeclaration;

// module-level object visitor
class ModuleDeclaration;
class ModuleFunction;
class IModuleStatementVisitor {
public:
  virtual void on(ModuleDeclaration *){};
  virtual void on(ModuleFunction *){};
};

// base class for all module-level statements
class ModuleStatement : public NodeOnToken {
public:
  ModuleStatement(const Lexer::Token *token = nullptr) : NodeOnToken(token) {}

  virtual void visit(IModuleStatementVisitor &) = 0;
};

class ModuleDeclaration : public ModuleStatement {
public:
  ModuleDeclaration(std::string_view moduleName,
                    const Lexer::Token *token = nullptr)
      : ModuleStatement(token), mModuleName(moduleName) {}

  std::string_view moduleName() const { return mModuleName; }

  void visit(IModuleStatementVisitor &v) override { v.on(this); }

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
  void visit(IModuleStatementVisitor &v) override { v.on(this); }
  FunctionDeclaration *function() { return mFunction; }

private:
  void doDump(int indent = 0) const override;

  FunctionDeclaration *mFunction = nullptr;
};

class Module : public Node {
public:
  void add(ModuleDeclaration *decl);
  void add(FunctionDeclaration *fun);

  const ModuleDeclaration *declaration() const { return mDeclaration; }

  Utils::ListView<ModuleStatement *> statements() { return mStatements; }

  std::string_view moduleName() const {
    if (mDeclaration)
      return mDeclaration->moduleName();
    else
      return {};
  }

private:
  void doDump(int indent = 0) const override;

  ModuleDeclaration *mDeclaration = nullptr;
  Utils::List<ModuleStatement *> mStatements;
};

} // namespace Cougar::Ast