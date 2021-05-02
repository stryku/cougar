#pragma once

#include "utils/diagnostics.hh"

namespace Cougar::Ast {
class Module;
class FunctionDeclaration;
class TypeNode;
class TypeName;
class Statement;
struct StGroup;
struct StFunctionCall;
} // namespace Cougar::Ast

namespace Cougar::Meta {
class Scope;
class TypeInfo;
} // namespace Cougar::Meta

namespace Cougar::Resolver {

// TODO move somwhere else
struct CompilationState {
  Meta::Scope *mBuildInScope = nullptr;
  Meta::Scope *mRootScope = nullptr;
};

class Resolver {
public:
  Resolver(Utils::Diagnostics &diag, CompilationState &state);

  void resolveModule(Ast::Module *module);

private:
  void resolveFunctionDelcaration(Ast::FunctionDeclaration *,
                                  Meta::Scope *scope);
  void resolveFunctionBody(Ast::FunctionDeclaration *, Meta::Scope *scope);

  Meta::TypeInfo *resolveType(Ast::TypeNode *tn, Meta::Scope *scope);
  Meta::TypeInfo *resolveNamedType(std::string_view name, Meta::Scope *scope);

  void resolveStatement(Ast::Statement *stmt, Meta::Scope *scope);

  void resolveStatementGroup(Ast::StGroup &stmt, Meta::Scope *groupScope);
  void resolveFunctionCall(Ast::StFunctionCall &stmt, Meta::Scope *scope);

  Utils::Diagnostics &mDiag;
  CompilationState &mState;
};

} // namespace Cougar::Resolver