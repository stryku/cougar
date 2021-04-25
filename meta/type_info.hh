#pragma once

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

  TypeInfo(Simple s) : mData(s) {}
  TypeInfo(Pointer p) : mData(p) {}

  void dump(int indent) const;

  TypeInfo *pointerType() { return mPointerType; }
  void setPointerType(TypeInfo *ptrType);

  bool isPointer() const { return std::holds_alternative<Pointer>(mData); }

private:
  std::variant<Pointer, Simple> mData;
  TypeInfo *mPointerType = nullptr;
};

} // namespace Cougar::Meta
