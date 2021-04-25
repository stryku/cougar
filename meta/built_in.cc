#include "built_in.hh"

#include "scope.hh"

#include "utils/zone_allocator.hh"

namespace Cougar::Meta {

using namespace Utils;

class _built_in_tag {};

Scope *createBuiltInScope() {

  Scope *builtIn = Zone::make<Scope>(_built_in_tag{});

  builtIn->addType(TypeInfo::Simple{"int8"});
  builtIn->addType(TypeInfo::Simple{"int16"});
  builtIn->addType(TypeInfo::Simple{"int32"});
  builtIn->addType(TypeInfo::Simple{"int64"});

  builtIn->addType(TypeInfo::Simple{"cstr"});

  return builtIn;
}

} // namespace Cougar::Meta