#include "parser_ast.hh"

#include <fmt/core.h>

namespace Cougar::ParserAST {

namespace {
constexpr int INDENT = 2;
}

INode::~INode() = default;

void Module::addNode(std::unique_ptr<INode> node) {
  mNodes.push_back(std::move(node));
}

void Module::dump(int indent) const {
  std::string is(std::size_t(indent), ' ');

  fmt::print("{:{}} Module\n", "", indent);

  for (auto &node : mNodes) {
    node->dump(indent + INDENT);
  }
}

void FunctionDefinition::dump(int indent) const {
  fmt::print("{:{}} FunctionDefinition: name={}, ret={}\n", "", indent, mName,
             mReturnType);
}

} // namespace Cougar::ParserAST