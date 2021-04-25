#include "resolver.hh"

#include "ast/module.hh"

#include "meta/scope.hh"

#include "utils/zone_allocator.hh"

#include <cassert>

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

class ModuleVisitor : public IModuleStatementVisitor {
public:
  void on(ModuleFunction *fun) override {
    mResolver->resolveFunctionDelcaration(fun->function(), mScope);
  }

  Resolver *mResolver;
  Scope *mScope;
};

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

  // Preparte visitor
  ModuleVisitor vtor;
  vtor.mResolver = this;
  vtor.mScope = scope;

  for (ModuleStatement *md : module->statements()) {
    md->visit(vtor);
  }
}

} // namespace Cougar::Resolver