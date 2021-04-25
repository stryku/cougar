#pragma once

#include "utils/diagnostics.hh"

namespace Cougar::Ast {
class Module;
}

namespace Cougar::Meta {
class Scope;
}

namespace Cougar::Resolver {

// TODO move somwhere else
struct CompilationState {
  const Meta::Scope *mBuildInScope = nullptr;
  Meta::Scope *mRootScope = nullptr;
};

class Resolver {
public:
  Resolver(Utils::Diagnostics &diag, CompilationState &state);

  void resolveModule(Ast::Module *module);

private:
  Utils::Diagnostics &mDiag;
  CompilationState &mState;
};

} // namespace Cougar::Resolver