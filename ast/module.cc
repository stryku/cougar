#include "module.hh"

#include "function.hh"
#include "statement.hh"

namespace Cougar::Ast {

void ModuleDeclaration::doDump(int indent) const {
  iprint(indent, "ModuleDeclaration(name={})", mModuleName);
}

Module::Module() { mScope = Utils::Zone::make<Scope>(); }

void Module::doDump(int indent) const {

  std::string_view name;
  if (mDeclaration)
    name = mDeclaration->moduleName();
  iprint(indent, "Module(name={})", name);
  if (mDeclaration) {
    iprint(indent + 2, "- declaration:");
    mDeclaration->dump(indent + 6);
  }
  iprint(indent + 2, "- functions:");
  for (const FunctionDeclaration *f : mFunctions) {
    f->dump(indent + 6);
  }
}

void Module::add(ModuleDeclaration *decl) {
  if (mDeclaration) {
    throw std::runtime_error("Repeated module declaration");
  }
  mDeclaration = decl;
}

void Module::add(FunctionDeclaration *fun) {
  assert(fun);
  mFunctions.emplace_back(fun);
}

} // namespace Cougar::Ast