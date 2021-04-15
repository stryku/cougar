#pragma once

#include "utils/list.hh"

namespace Cougar::Ast {

class Node {
public:
  virtual void dump(std::string_view indent = {}) = 0;

private:
  Utils::List<Node> mChildren;
};

} // namespace Cougar::Ast