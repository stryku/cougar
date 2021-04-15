#include "module.hh"

#include <fmt/core.h>

namespace Cougar::Ast {

void Module::dump(std::string_view indent) { fmt::print("{}Module\n", indent); }

} // namespace Cougar::Ast