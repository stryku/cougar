#include "function.hh"

namespace Cougar::Ast {

void TypeName::doDump(int indent) const {
  iprint(indent, "TypeName(name={})", mTypeName);
}

void FunctionDeclaration::doDump(int indent) const {
  iprint(indent, "FunctionDeclaration(access={}, name={})", mAccess, mName);
  iprint(indent + 2, "- return type:");
  mReturnType->dump(indent + 6);
  iprint(indent + 2, "- args:");
  for (const FunctionArg &arg : mArgs) {
    arg.dump(indent + 6);
  }
}

void FunctionDeclaration::addArg(TypeName *argType, std::string_view argName) {
  mArgs.emplace_back(argType, argName);
}

void FunctionArg::doDump(int indent) const {
  iprint(indent, "FunctionArg(type={}, name={})", mType->typeName(), mName);
}

} // namespace Cougar::Ast