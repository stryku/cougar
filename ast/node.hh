#pragma once

#include "lexer/token.hh"

#include "utils/list.hh"

namespace Cougar::Ast {

class Node {
public:
  void dump(int indent = 0) const {
    doDump(indent);
    for (const Node *node : mChildren) {
      node->dump(indent + 4);
    }
  }

protected:
  virtual void doDump(int indent) const = 0;
  Utils::List<Node *> mChildren;
};

// dump helper: printd line with indentation
template <typename... Args>
void iprint(int indent, std::string_view fmt, const Args &...args) {
  if (indent > 0)
    fmt::print("{:{}}", " ", indent);
  fmt::print(fmt, args...);
  fmt::print("\n");
}

} // namespace Cougar::Ast