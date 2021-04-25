#include "type_info.hh"

#include "utils/iprint.hh"

namespace Cougar::Meta {

using namespace Utils;

void TypeInfo::dump(int indent) const { iprint(indent, "Type({})", mName); }

} // namespace Cougar::Meta