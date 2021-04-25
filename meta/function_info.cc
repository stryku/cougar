#include "function_info.hh"

#include "type_info.hh"

#include "utils/iprint.hh"

namespace Cougar::Meta {

using namespace Utils;

void FunctionInfo::dump(int indent) const {
  iprint(indent, "Function({})", name);
  iprint(indent + 2, "- return type: {}", returnType->prettyName());
  iprint(indent + 2, "- args:");
  for (const Arg &arg : args) {
    iprint(indent + 6, "{} : {}", arg.name, arg.type->prettyName());
  }
}

} // namespace Cougar::Meta