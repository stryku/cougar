#pragma once

#include "utils/list.hh"
#include "utils/source_location.hh"

#include <string_view>

namespace Cougar::Meta {

class Scope;
class TypeInfo;

class FunctionInfo {
public:
  struct Arg {
    std::string_view name;
    TypeInfo *type;
  };

  FunctionInfo(std::string_view name, TypeInfo *retType,
               Utils::SourceLocation loc)
      : mName(name), mReturnType(retType), mLocation(loc) {}

  std::string_view name() const { return mName; }
  TypeInfo *returnType() { return mReturnType; }

  void addArg(std::string_view name, TypeInfo *type) {
    mArgs.emplace_back(Arg{name, type});
  }

  const Utils::SourceLocation &location() const { return mLocation; }

private:
  std::string_view mName;
  TypeInfo *mReturnType = nullptr;
  Utils::List<Arg> mArgs;
  Utils::SourceLocation mLocation;
};

} // namespace Cougar::Meta
