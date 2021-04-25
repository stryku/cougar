#include "resolver.hh"

#include "ast/type.hh"

#include "meta/scope.hh"
#include "meta/type_info.hh"

#include "utils/zone_allocator.hh"

#include <cassert>

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

class TypeVisitor : public Ast::IPointerNodeVisitor {
public:
  TypeVisitor(Resolver *r, Scope *s) : mResolver(r), mScope(s) {}

  void on(Ast::TypeName *nameNode) {
    mResult = mResolver->resolveTypeName(nameNode, mScope);
  }

  void on(Ast::PointerTo *ptrNode) {
    ptrNode->pointedType()->visit(*this);
    if (mResult) {
      Meta::TypeInfo *ptr = mResult->pointerType();
      if (!ptr) {
        Meta::TypeInfo::Pointer p;
        p.pointed = mResult;
        ptr = Zone::make<Meta::TypeInfo>(p);
      }
      mResult = ptr;
    }
  }

  Resolver *mResolver = nullptr;
  Scope *mScope = nullptr;
  Meta::TypeInfo *mResult = nullptr;
};

Meta::TypeInfo *Resolver::resolveType(Ast::TypeNode *tn, Meta::Scope *scope) {

  TypeVisitor vtor{this, scope};

  tn->visit(vtor);
  return vtor.mResult;
}

Meta::TypeInfo *Resolver::resolveTypeName(Ast::TypeName *tn,
                                          Meta::Scope *scope) {

  std::string_view name = tn->name();

  // TODO
  (void)name;
  (void)scope;
  return nullptr;
}

} // namespace Cougar::Resolver