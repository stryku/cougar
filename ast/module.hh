#pragma once

#include "node.hh"

namespace Cougar::Ast {

class Module : public Node {
public:
  void dump(std::string_view indent = {}) override;
};

} // namespace Cougar::Ast