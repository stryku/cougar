#pragma once

#include "cookie.hh"

#include "utils/list.hh"
#include "utils/source_location.hh"

#include <string_view>

namespace Cougar::Meta {

class Scope;
class TypeInfo;

class FunctionInfo {
public:
  struct Arg { // TODO: maybe this should be VariableInfo ?
    std::string_view name;
    TypeInfo *type;
  };

  FunctionInfo(std::string_view name) : name(name) {}

  const std::string_view name;
  TypeInfo *returnType = nullptr;
  Utils::List<Arg> args;
  Utils::SourceLocation location;
  Cookie codegenData;

  void dump(int indent) const;
};

} // namespace Cougar::Meta
