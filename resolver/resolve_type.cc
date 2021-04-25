#include "resolver.hh"

#include "ast/type.hh"

#include "meta/scope.hh"
#include "meta/type_info.hh"

#include "utils/overloaded.hh"
#include "utils/zone_allocator.hh"

#include <cassert>

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

Meta::TypeInfo *Resolver::resolveType(TypeNode *tn, Meta::Scope *scope) {

  Meta::TypeInfo *resolved = tn->visit( //
      overloaded                        //
      {                                 // pointer
       [&](const TypeNode::Pointer &ptr) {
         // recursively resolve pointed type
         Meta::TypeInfo *ti = resolveType(ptr.pointedType, scope);
         Meta::TypeInfo *ptrTi = ti->pointerType();
         if (!ptrTi) {
           Meta::TypeInfo::Pointer p;
           p.pointed = ti;
           ptrTi = Zone::make<Meta::TypeInfo>(p);
         }
         return ptrTi;
       },
       // named
       [&](const TypeNode::Named &n) {
         Meta::TypeInfo *ti = resolveNamedType(n.name, scope);
         if (!ti) {
           if (tn->token()) {
             mDiag.error(tn->token()->location, "Unknown type '{}'", n.name);
           } else {
             // TODO - how to report an error?
             throw std::runtime_error(fmt::format("Unknown type '{}'", n.name));
           }
         }
         return ti;
       }});

  // TODO assign resolved type to token, if present
  return resolved;
}

Meta::TypeInfo *Resolver::resolveNamedType(std::string_view name,
                                           Meta::Scope *scope) {

  // see if it may be a built-in type
  TypeInfo *ti = mState.mBuildInScope->findType(name);
  if (ti)
    return ti;

  // traverse local scopes
  for (; scope->parent(); scope = scope->parent()) {
    TypeInfo *ti = scope->findType(name);
    if (ti)
      return ti;
  }
  return nullptr;
}

} // namespace Cougar::Resolver