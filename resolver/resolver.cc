#include "resolver.hh"

#include "ast/module.hh"

#include "meta/scope.hh"

#include "utils/overloaded.hh"
#include "utils/zone_allocator.hh"

#include <cassert>

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

Resolver::Resolver(Utils::Diagnostics &diag, CompilationState &state)
    : mDiag(diag), mState(state) {}

void Resolver::resolveModule(Module *module) {

  assert(module);

  // Prepare scope

  std::string_view moduleName = module->moduleName();
  Scope *scope = mState.mRootScope;
  if (!moduleName.empty()) {
    // create named scope for module
    scope = Zone::make<Scope>(moduleName, scope);
  }
  // else use root scope

  // Resolve function declarations
  for (ModuleStatement &md : module->statements()) {
    md.visit(overloaded //
             {          //
              [&](FunctionDeclaration *fun) {
                resolveFunctionDelcaration(fun, scope);
              },
              [](auto &) {}});
  }

  // 2nd pass: resolve function bodies
  for (ModuleStatement &md : module->statements()) {
    md.visit(
        overloaded //
        {          //
         [&](FunctionDeclaration *fun) { resolveFunctionBody(fun, scope); },
         [](auto &) {}});
  }
}

} // namespace Cougar::Resolver