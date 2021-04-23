#pragma once

#include "utils/diagnostics.hh"

namespace Cougar::Ast {
class Module;
}

namespace Cougar::Resolver {

class Resolver {
public:
  Resolver(Utils::Diagnostics &diag);

  void resolveModule(Ast::Module *module);

private:
  Utils::Diagnostics &mDiag;
};

} // namespace Cougar::Resolver