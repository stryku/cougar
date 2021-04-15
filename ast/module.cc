#include "module.hh"

#include "function.hh"

namespace Cougar::Ast {

void ModuleDeclaration::doDump(int indent) const {
  iprint(indent, "ModuleDeclaration(name={})", mModuleName);
}

void Module::doDump(int indent) const {

  std::string_view name;
  if (mDeclaration)
    name = mDeclaration->moduleName();
  iprint(indent, "Module(name={})", name);
  if (mDeclaration) {
    iprint(indent + 2, "- declaration:");
    mDeclaration->dump(indent + 4);
  }
  iprint(indent + 2, "- functions:");
  for (const FunctionDeclaration *f : mFunctions) {
    f->dump(indent + 4);
  }
}

void Module::add(ModuleDeclaration *decl) {
  if (mDeclaration) {
    throw std::runtime_error("Repeated module declaration");
  }
  mDeclaration = decl;
}

void Module::add(Utils::ZoneAllocator &zone, FunctionDeclaration *fun) {
  assert(fun);
  mFunctions.emplace_back(zone, fun);
}

} // namespace Cougar::Ast