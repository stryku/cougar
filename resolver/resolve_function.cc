#include "resolver.hh"

#include "ast/function.hh"
#include "ast/type.hh"

#include "meta/scope.hh"

#include <cassert>

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

void Resolver::resolveFunctionDelcaration(Ast::FunctionDeclaration *decl,
                                          Meta::Scope *scope) {
  assert(decl);
  assert(scope);

  // resolve return type
  TypeNode *rtn = decl->returnType();
  TypeInfo *rti = resolveType(rtn, scope);

  // TODO
  (void)rti;
}

} // namespace Cougar::Resolver
