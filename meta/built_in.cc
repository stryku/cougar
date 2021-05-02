#include "built_in.hh"

#include "scope.hh"

#include "utils/zone_allocator.hh"

namespace Cougar::Meta {

namespace BuiltIn {

TypeInfo *typeCStr = nullptr;
TypeInfo *typeInt8 = nullptr;
TypeInfo *typeInt16 = nullptr;
TypeInfo *typeInt32 = nullptr;
TypeInfo *typeInt64 = nullptr;

} // namespace BuiltIn

using namespace Utils;

class _built_in_tag {};

Scope *createBuiltInScope() {

  Scope *builtIn = Zone::make<Scope>(_built_in_tag{});

  BuiltIn::typeInt8 = builtIn->addType(TypeInfo::Simple{"int8"});
  BuiltIn::typeInt16 = builtIn->addType(TypeInfo::Simple{"int16"});
  BuiltIn::typeInt32 = builtIn->addType(TypeInfo::Simple{"int32"});
  BuiltIn::typeInt64 = builtIn->addType(TypeInfo::Simple{"int64"});

  BuiltIn::typeCStr = builtIn->addType(TypeInfo::Simple{"cstr"});

  return builtIn;
}

} // namespace Cougar::Meta