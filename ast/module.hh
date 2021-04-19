#pragma once

#include "node.hh"

namespace Cougar::Ast {

class FunctionDeclaration;
class Scope;

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
  Module();

  void add(ModuleDeclaration *decl);
  void add(FunctionDeclaration *fun);

  const ModuleDeclaration *declaration() const { return mDeclaration; }

  Scope *scope() { return mScope; }

private:
  void doDump(int indent = 0) const override;

  ModuleDeclaration *mDeclaration = nullptr;
  Utils::List<FunctionDeclaration *> mFunctions;
  Scope *mScope;
};

} // namespace Cougar::Ast