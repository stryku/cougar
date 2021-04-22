#include "module.hh"

#include "function.hh"
#include "statement.hh"

namespace Cougar::Ast {

using namespace Utils;

void ModuleDeclaration::doDump(int indent) const {
  iprint(indent, "ModuleDeclaration(name={})", mModuleName);
}

void ModuleFunction::doDump(int indent) const { mFunction->dump(indent); }

void Module::doDump(int indent) const {

  std::string_view name;
  if (mDeclaration)
    name = mDeclaration->moduleName();
  iprint(indent, "Module(name={})", name);
  if (mDeclaration) {
    iprint(indent + 2, "- declaration:");
    mDeclaration->dump(indent + 6);
  }
  iprint(indent + 2, "- statements:");
  for (const ModuleStatement *s : mStatements) {
    s->dump(indent + 6);
  }
}

void Module::add(ModuleDeclaration *decl) {
  assert(decl);
  if (mDeclaration) {
    throw std::runtime_error("Repeated module declaration");
  }
  mDeclaration = decl;
  mStatements.emplace_back(decl);
}

void Module::add(FunctionDeclaration *fun) {
  assert(fun);
  ModuleFunction *mf = Zone::make<ModuleFunction>(fun);
  mStatements.emplace_back(mf);
}

} // namespace Cougar::Ast