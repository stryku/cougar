#include "scope.hh"

#include "utils/iprint.hh"

namespace Cougar::Meta {

using namespace Utils;

void Scope::dump(int indent) const {

  if (mIsBuiltIn) {
    iprint(indent, "Scope (<built in>)");
  } else if (mName.empty()) {
    iprint(indent, "Scope (<root>)", mName);
  } else {
    iprint(indent, "Scope ({})", mName);
  }

  if (!mTypes.empty()) {
    iprint(indent + 2, "- types");
    mTypes.for_each(
        [&](const auto &, const TypeInfo &i) { i.dump(indent + 6); });
  }

  if (!mFunctions.empty()) {
    iprint(indent + 2, "- functions");
    mFunctions.for_each(
        [&](const auto &, const FunctionInfo &i) { i.dump(indent + 6); });
  }

  if (!mChildren.empty()) {
    iprint(indent + 2, "- children");
    mChildren.for_each(
        [&](const auto &, const Scope *i) { i->dump(indent + 4); });
  }
}

} // namespace Cougar::Meta