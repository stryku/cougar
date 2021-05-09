#include "resolver.hh"

#include "ast/expression.hh"

#include "meta/built_in.hh"
#include "meta/scope.hh"

#include "utils/overloaded.hh"

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

void Resolver::resolveExpression(Ast::Expression *e, Meta::Scope *scope) {

  (void)scope;
  assert(e);

  e->visit(overloaded{
      [&](const ExStringLiteral &) { e->setType(BuiltIn::typeCStr); },
      [&](const ExNumberLiteral &) { /* TODO */ }});
}

} // namespace Cougar::Resolver