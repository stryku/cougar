#include "resolver.hh"

#include "ast/function.hh"
#include "ast/type.hh"

#include "meta/function_info.hh"
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

  // create function object, if no error
  // FunctionInfo *result = nullptr;
  // if (rti) {
  //   std::string_view name = decl->name();
  //   FunctionInfo *collision = scope->findFunction(name);
  //   if (collision) {
  //     mDiag.error(decl->token()->location, "Function '{}' already defined");
  //     mDiag.error(collision->location(), "Previously defined here");
  //   } else {
  //     result = scope->addFunction(
  //   }
  // }
  // TODO  no no no
  //
  // - what about overloads with different types? check here!
  // - allows for resolving args even if function name is invalid. Before
  // function name is checked!

  
  // TODO
  (void)rti;
}

} // namespace Cougar::Resolver
