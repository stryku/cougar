#pragma once

#include "node.hh"

namespace Cougar::Ast {

class ModuleDeclaration : public Node {
public:
  ModuleDeclaration(std::string_view moduleName,
                    const Lexer::Token *token = nullptr)
      : mModuleName(moduleName), mToken(token) {}

  std::string_view moduleName() const { return mModuleName; }
  const Lexer::Token *token() const { return mToken; }

private:
  void doDump(int indent = 0) const override;

  std::string_view mModuleName;
  const Lexer::Token *mToken;
};

class Module : public Node {
public:
  void add(Utils::ZoneAllocator &zone, ModuleDeclaration *decl);

  const ModuleDeclaration *declaration() const { return mDeclaration; }

private:
  void doDump(int indent = 0) const override;

  ModuleDeclaration *mDeclaration = nullptr;
};

} // namespace Cougar::Ast