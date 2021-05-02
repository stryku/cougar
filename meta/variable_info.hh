#pragma once

#include "utils/source_location.hh"

#include <string_view>

namespace Cougar::Meta {

class TypeInfo;

class VariableInfo {
public:
  VariableInfo(std::string_view name) : name(name) {}

  std::string_view name;
  TypeInfo *type = nullptr;
  Utils::SourceLocation location;
};

} // namespace Cougar::Meta