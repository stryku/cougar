#include "resolver.hh"

#include "ast/expression.hh"

#include "meta/built_in.hh"
#include "meta/scope.hh"

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

void Resolver::resolveExpression(Ast::Expression *e, Meta::Scope *scope) {

  (void)scope;
  assert(e);

  e->visit([&](const ExStringLiteral &) { e->setType(BuiltIn::typeCStr); });
}

} // namespace Cougar::Resolver