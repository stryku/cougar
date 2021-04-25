#include "resolver.hh"

namespace Cougar::Resolver {

Resolver::Resolver(Utils::Diagnostics &diag, CompilationState &state)
    : mDiag(diag), mState(state) {}

void Resolver::resolveModule(Ast::Module *module) {
  // TODO
  (void)module;
}

} // namespace Cougar::Resolver