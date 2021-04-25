#include "resolver.hh"

#include "ast/function.hh"
#include "ast/type.hh"

#include "meta/function_info.hh"
#include "meta/scope.hh"

#include <algorithm>
#include <cassert>

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

namespace {
bool areArgTypesSame(const Utils::List<FunctionInfo::Arg> &args1,
                     const Utils::List<FunctionInfo::Arg> &args2) {
  return std::equal(args1.begin(),
                    args1.end(), //
                    args2.begin(),
                    args2.end(), //
                    [](auto &a, auto &b) { return a.type == b.type; });
}

} // namespace

void Resolver::resolveFunctionDelcaration(Ast::FunctionDeclaration *decl,
                                          Meta::Scope *scope) {
  assert(decl);
  assert(scope);

  // resolve args
  Utils::List<FunctionInfo::Arg> args;
  bool ok = true;
  for (FunctionArg &fa : decl->args()) {
    TypeInfo *rti = resolveType(fa.type, scope);
    if (rti && ok) {
      args.emplace_back(FunctionInfo::Arg{fa.name, rti});
    } else {
      ok = false;
    }
  }

  // resolve return type
  // TODO support for 'auto'
  TypeNode *rtn = decl->returnType();
  TypeInfo *rti = resolveType(rtn, scope);

  if (!rti)
    ok = false;

  if (ok) {

    // check for collision
    std::string_view name = decl->name();
    FunctionInfo *collision = scope->findFunction(name);

    // compare types
    if (collision && areArgTypesSame(collision->args, args)) {
      mDiag.error(decl->token()->location, "Function '{}' already defined",
                  name);
      mDiag.error(collision->location, "Previously defined here");
    } else {

      FunctionInfo *result = scope->addFunction(name);
      result->location = decl->token()->location;
      result->args = args;
      result->returnType = rti;
    }
  }
}

} // namespace Cougar::Resolver
