#include "built_in.hh"

#include "scope.hh"

#include "utils/zone_allocator.hh"

namespace Cougar::Meta {

using namespace Utils;

class _built_in_tag {};

const Scope *createBuiltInScope() {

  Scope *builtIn = Zone::make<Scope>(_built_in_tag{});

  builtIn->addType("int8");
  builtIn->addType("int16");
  builtIn->addType("int32");
  builtIn->addType("int64");

  builtIn->addType("cstr");

  return builtIn;
}

} // namespace Cougar::Meta