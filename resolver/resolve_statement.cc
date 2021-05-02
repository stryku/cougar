#include "resolver.hh"

#include "ast/statement.hh"

#include "meta/scope.hh"

#include "utils/overloaded.hh"

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

void Resolver::resolveStatementGroup(StGroup &grp, Meta::Scope *groupScope) {

  for (Statement *stmt : grp.statements) {
    resolveStatement(stmt, groupScope);
  }
}

void Resolver::resolveFunctionCall(StFunctionCall &stmt, Meta::Scope *scope) {

  // TODO
  // - resolve arguments
  // - find function by name
  // - TODO - perform overload resolution
  // - inject cast expression if required

  (void)scope;
  (void)stmt;
}

void Resolver::resolveStatement(Ast::Statement *stmt, Meta::Scope *scope) {

  stmt->visit(
      overloaded{[&](StGroup &s) {
                   // create new scope for the group
                   Scope *groupScope = Zone::make<Scope>("", scope);
                   resolveStatementGroup(s, groupScope);
                 },
                 [&](StFunctionCall &s) { resolveFunctionCall(s, scope); }});
}

} // namespace Cougar::Resolver