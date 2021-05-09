#include "resolver.hh"

#include "ast/expression.hh"
#include "ast/statement.hh"

#include "meta/scope.hh"

#include "utils/overloaded.hh"

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

void Resolver::resolveStatementGroup(StGroup &grp, Meta::Scope *groupScope) {

  assert(!grp.scope);
  grp.scope = groupScope;
  for (Statement *stmt : grp.statements) {
    resolveStatement(stmt, groupScope);
  }
}

Meta::FunctionInfo *Resolver::resolveNamedFunction(std::string_view name,
                                                   Meta::Scope *scope) {
  FunctionInfo *fi = mState.mBuildInScope->findFunction(name);
  if (fi)
    return fi;

  // traverse local scopes
  for (; scope->parent(); scope = scope->parent()) {
    FunctionInfo *fi = scope->findFunction(name);
    if (fi)
      return fi;
  }
  return nullptr;
}

void Resolver::resolveFunctionCall(const Utils::SourceLocation &loc,
                                   StFunctionCall &stmt, Meta::Scope *scope) {

  // - resolve arguments
  for (Expression *e : stmt.params->params()) {
    resolveExpression(e, scope);
  }

  // - find function by name
  FunctionInfo *fi = resolveNamedFunction(stmt.name, scope);
  if (!fi) {
    mDiag.error(loc, "Unknown function '{}'", stmt.name);
    return;
  }

  // TODO
  // - TODO - perform overload resolution
  // - inject cast expression if required

  // for now, just check if param types match
  auto it1 = fi->args.begin();
  auto it2 = stmt.params->params().begin();
  int idx = 0;
  while (it1 != fi->args.end() && it2 != stmt.params->params().end()) {
    idx++;
    if (it1->type != (*it2)->type()) {
      mDiag.error(loc, "Function call parameter type mismatch");
      mDiag.error(loc, "Parameter #{} expects type '{}', got '{}'",
                  it1->type->prettyName(), (*it2)->type()->prettyName());
      return;
    }
    ++it1;
    ++it2;
  }

  if (it1 != fi->args.end()) {
    mDiag.error(loc, "Too few paramters passed to function call");
    return;
  } else if (it2 != stmt.params->params().end()) {
    mDiag.error(loc, "Too many paramters passed to function call");
    return;
  }

  stmt.info = fi;
}

void Resolver::resolveStatement(Ast::Statement *stmt, Meta::Scope *scope) {

  stmt->visit(overloaded{[&](StGroup &s) {
                           // create new scope for the group
                           Scope *groupScope = Zone::make<Scope>("", scope);
                           resolveStatementGroup(s, groupScope);
                         },
                         [&](StFunctionCall &s) {
                           resolveFunctionCall(stmt->token()->location, s,
                                               scope);
                         },
                         [&](StReturn &r) {
                           // TODO
                           if (r.expression)
                             resolveExpression(r.expression, scope);
                         }});
}

} // namespace Cougar::Resolver