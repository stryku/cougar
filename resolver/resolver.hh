#pragma once

#include "utils/diagnostics.hh"

namespace Cougar::Ast {
class Module;
class FunctionDeclaration;
class TypeNode;
class TypeName;
} // namespace Cougar::Ast

namespace Cougar::Meta {
class Scope;
class TypeInfo;
} // namespace Cougar::Meta

namespace Cougar::Resolver {

// TODO move somwhere else
struct CompilationState {
  const Meta::Scope *mBuildInScope = nullptr;
  Meta::Scope *mRootScope = nullptr;
};

class Resolver {
public:
  Resolver(Utils::Diagnostics &diag, CompilationState &state);

  void resolveModule(Ast::Module *module);

private:
  friend class ModuleVisitor;
  friend class TypeVisitor;

  void resolveFunctionDelcaration(Ast::FunctionDeclaration *,
                                  Meta::Scope *scope);

  Meta::TypeInfo *resolveType(Ast::TypeNode *tn, Meta::Scope *scope);
  Meta::TypeInfo *resolveTypeName(Ast::TypeName *tn, Meta::Scope *scope);

  Utils::Diagnostics &mDiag;
  CompilationState &mState;
};

} // namespace Cougar::Resolver