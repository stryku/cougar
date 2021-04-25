#pragma once

#include "node.hh"

#include <variant>

namespace Cougar::Ast {

class FunctionDeclaration;
class ModuleDeclaration;

// base class for all module-level statements
class ModuleStatement {
public:
  ModuleStatement(FunctionDeclaration *f) : mData(f) {}
  ModuleStatement(ModuleDeclaration *m) : mData(m) {}

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

  void dump(int indent = 0) const;

private:
  std::variant<FunctionDeclaration *, ModuleDeclaration *> mData;
};

class ModuleDeclaration : public NodeOnToken {
public:
  ModuleDeclaration(std::string_view moduleName,
                    const Lexer::Token *token = nullptr)
      : NodeOnToken(token), mModuleName(moduleName) {}

  std::string_view moduleName() const { return mModuleName; }

private:
  void doDump(int indent = 0) const override;
  std::string_view mModuleName;
};

class Module : public Node {
public:
  void add(ModuleDeclaration *decl);
  void add(FunctionDeclaration *fun);

  const ModuleDeclaration *declaration() const { return mDeclaration; }

  Utils::ListView<ModuleStatement> statements() { return mStatements; }

  std::string_view moduleName() const {
    if (mDeclaration)
      return mDeclaration->moduleName();
    else
      return {};
  }

private:
  void doDump(int indent = 0) const override;

  ModuleDeclaration *mDeclaration = nullptr;
  Utils::List<ModuleStatement> mStatements;
};

} // namespace Cougar::Ast