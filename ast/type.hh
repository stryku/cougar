#pragma once

#include "node.hh"

namespace Cougar::Ast {

// abstract class for all type nodes
class TypeNode : public NodeOnToken {
public:
  TypeNode(const Lexer::Token *tok = nullptr) : NodeOnToken(tok) {}
  virtual std::string name() = 0; // rendered name for in-line printing
};

// leaf - an actual type identifier
class TypeName : public TypeNode {
public:
  TypeName(std::string_view name, const Lexer::Token *tok = nullptr)
      : TypeNode(tok), mTypeName(name) {}

  std::string_view typeName() const { return mTypeName; }
  std::string name() override { return std::string(mTypeName); }

private:
  void doDump(int indent = 0) const override;

  std::string_view mTypeName;
  const Lexer::Token *mToken;
};

// pointer
class PointerTo : public TypeNode {
public:
  PointerTo(TypeNode *pointee, const Lexer::Token *tok = nullptr)
      : TypeNode(tok), mPointee(pointee) {
    assert(pointee);
  }

  std::string name() override { return fmt::format("{}*", mPointee->name()); }

private:
  void doDump(int indent = 0) const override;
  TypeNode *mPointee;
};

} // namespace Cougar::Ast