#include "resolver.hh"

#include "ast/function.hh"
#include "ast/statement.hh"
#include "ast/type.hh"

#include "meta/function_info.hh"
#include "meta/scope.hh"
#include "meta/variable_info.hh"

#include "utils/overloaded.hh"

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
                                          Meta::Scope *parentScope) {
  assert(decl);
  assert(parentScope);

  // resolve args
  Utils::List<FunctionInfo::Arg> args;
  bool ok = true;
  for (FunctionArg &fa : decl->args()) {
    TypeInfo *ti = resolveType(fa.type, parentScope);
    if (ti && ok) {
      args.emplace_back(FunctionInfo::Arg{fa.name, ti});
      fa.type->setInfo(ti);
    } else {
      ok = false;
    }
  }

  // resolve return type
  // TODO support for 'auto'
  TypeNode *rtn = decl->returnType();
  TypeInfo *rti = resolveType(rtn, parentScope);

  if (!rti) {
    ok = false;
  } else {
    rtn->setInfo(rti);
  }

  if (ok) {

    // check for collision
    std::string_view name = decl->name();
    FunctionInfo *collision = parentScope->findFunction(name);

    // compare types
    if (collision && areArgTypesSame(collision->args, args)) {
      mDiag.error(decl->token()->location, "Function '{}' already defined",
                  name);
      mDiag.error(collision->location, "Previously defined here");
    } else {

      FunctionInfo *result = parentScope->addFunction(name);
      result->location = decl->token()->location;
      result->args = args;
      result->returnType = rti;

      decl->setInfo(result);
    }
  }
}

void Resolver::resolveFunctionBody(Ast::FunctionDeclaration *decl,
                                   Meta::Scope *parentScope) {

  assert(decl);
  assert(parentScope);

  // does the function have body?
  // was the function declaration successfully resolved?
  if (!decl->body() || !decl->info())
    return;

  // create a Scope for the function
  Scope *functionScope = Zone::make<Scope>("", parentScope);

  // create variables
  for (auto arg : decl->args()) {

    // function param can be nameless, which means it is unused
    if (arg.name.empty())
      continue;

    assert(arg.type);

    // ensure the arg is not unique
    VariableInfo *collision = functionScope->findVariable(arg.name);
    if (collision) {
      mDiag.error(arg.type->token()->location,
                  "Redefinition of function argument '{}'", arg.name);
      mDiag.error(collision->location, "Previous definition here");
      continue;
    }

    // if arg type is invalid, skip
    TypeInfo *argTypeInfo = arg.type->info();
    if (!argTypeInfo)
      continue;

    auto var = functionScope->addVariable(arg.name);
    var->type = argTypeInfo;
    var->location = arg.type->token()->location;
  }

  // new we have a prepared scope, we can resolve statements in it.
  Ast::Statement *body = decl->body();

  body->visit(overloaded{
      [&](Ast::StGroup &group) { resolveStatementGroup(group, functionScope); },
      [](auto) {
        throw std::logic_error("Function body must be a statement group");
      }});
}

} // namespace Cougar::Resolver
