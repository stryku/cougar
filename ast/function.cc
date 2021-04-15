#include "function.hh"

namespace Cougar::Ast {

void Type::doDump(int indent) const {
  iprint(indent, "Type(name={})", mTypeName);
}

void FunctionDeclaration::doDump(int indent) const {
  iprint(indent, "FunctionDeclaration(access={}, name={})", mAccess, mName);
  iprint(indent + 2, "- return type:");
  mReturnType->dump(indent + 4);
  iprint(indent + 2, "- args:");
  for (const FunctionArg &arg : mArgs) {
    arg.dump(indent + 4);
  }
}

void FunctionDeclaration::addArg(Utils::ZoneAllocator &zone, Type *argType,
                                 std::string_view argName) {
  mArgs.emplace_back(zone, argType, argName);
}

void FunctionArg::doDump(int indent) const {
  iprint(indent, "FunctionArg(type={}, name={})", mType->typeName(), mName);
}

} // namespace Cougar::Ast