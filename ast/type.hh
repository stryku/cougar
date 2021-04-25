#pragma once

#include "node.hh"

#include <variant>

namespace Cougar::Ast {

// type node
class TypeNode : public NodeOnToken {
public:
  struct Pointer {
    TypeNode *mPointedType;
  };

  struct Named {
    std::string_view mName;
  };

  TypeNode(std::string_view name, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(Named{name}) {}

  TypeNode(TypeNode *pointedType, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(Pointer{pointedType}) {}

  std::string name() const; // rendered name for in-line printing

private:
  void doDump(int indent) const override;
  std::variant<Pointer, Named> mData;
};

} // namespace Cougar::Ast