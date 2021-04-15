#include "module.hh"

namespace Cougar::Ast {

void ModuleDeclaration::doDump(int indent) const {
  iprint(indent, "ModuleDeclaration(name={})", mModuleName);
}

void Module::doDump(int indent) const { iprint(indent, "Module"); }

void Module::add(Utils::ZoneAllocator &zone, ModuleDeclaration *decl) {
  if (mDeclaration) {
    throw std::runtime_error("Repeated module declaration");
  }
  mDeclaration = decl;
  mChildren.emplace_back(zone, decl);
}

} // namespace Cougar::Ast