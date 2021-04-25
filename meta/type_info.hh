#pragma once

#include "utils/zone_allocator.hh"

#include <string_view>
#include <variant>

namespace Cougar::Meta {

class Scope;

class TypeInfo {
public:
  struct Pointer {
    TypeInfo *pointed = nullptr;
  };

  struct Simple {
    std::string_view mName;
  };

  TypeInfo(Simple s) : mData(s) { mPrettyName = s.mName; }
  TypeInfo(Pointer p) : mData(p) {
    mPrettyName = Utils::Zone::format("{}*", p.pointed->prettyName());
  }

  void dump(int indent) const;

  TypeInfo *pointerType() { return mPointerType; }
  void setPointerType(TypeInfo *ptrType);

  bool isPointer() const { return std::holds_alternative<Pointer>(mData); }

  std::string_view prettyName() const { return mPrettyName; }

private:
  std::variant<Pointer, Simple> mData;
  TypeInfo *mPointerType = nullptr;
  std::string_view mPrettyName;
};

} // namespace Cougar::Meta
