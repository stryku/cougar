#pragma once

#include <string_view>

#include "type_info.hh"

#include "utils/map.hh"

namespace Cougar::Meta {

class _built_in_tag;

class Scope {
public:
  Scope(std::string_view name, Scope *parent) : mName(name), mParent(parent) {}

  Scope(const _built_in_tag &) { mIsBuiltIn = true; }

  void dump(int indent = 0) const;

  TypeInfo *addType(std::string_view name) {
    return mTypes.emplace(name, name);
  }

private:
  std::string_view mName;
  Scope *mParent = nullptr;
  bool mIsBuiltIn = false;

  Utils::Map<std::string_view, TypeInfo> mTypes;
};

} // namespace Cougar::Meta
