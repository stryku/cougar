#pragma once

#include "node.hh"

namespace Cougar::Ast {

// type visitior
class TypeName;
class PointerTo;
class IPointerNodeVisitor {
public:
  virtual void on(TypeName *) {}
  virtual void on(PointerTo *) {}
};

// abstract class for all type nodes
class TypeNode : public NodeOnToken {
public:
  TypeNode(const Lexer::Token *tok = nullptr) : NodeOnToken(tok) {}
  virtual std::string name() = 0; // rendered name for in-line printing
  virtual void visit(IPointerNodeVisitor &) = 0;
};

// leaf - an actual type identifier
class TypeName : public TypeNode {
public:
  TypeName(std::string_view name, const Lexer::Token *tok = nullptr)
      : TypeNode(tok), mTypeName(name) {}

  std::string_view typeName() const { return mTypeName; }
  std::string name() override { return std::string(mTypeName); }
  void visit(IPointerNodeVisitor &v) override { v.on(this); }

private:
  void doDump(int indent = 0) const override;

  std::string_view mTypeName;
  const Lexer::Token *mToken;
};

// pointer
class PointerTo : public TypeNode {
public:
  PointerTo(TypeNode *pointedType, const Lexer::Token *tok = nullptr)
      : TypeNode(tok), mPointedType(pointedType) {
    assert(pointedType);
  }

  std::string name() override {
    return fmt::format("{}*", mPointedType->name());
  }
  void visit(IPointerNodeVisitor &v) override { v.on(this); }

  TypeNode *pointedType() { return mPointedType; }

private:
  void doDump(int indent = 0) const override;
  TypeNode *mPointedType;
};

} // namespace Cougar::Ast