#pragma once

#include "node.hh"

#include <variant>

namespace Cougar::Meta {
class TypeInfo;
}

namespace Cougar::Ast {

// type node
class TypeNode : public NodeOnToken {
public:
  struct Pointer {
    TypeNode *pointedType;
  };

  struct Named {
    std::string_view name;
  };

  TypeNode(std::string_view name, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(Named{name}) {}

  TypeNode(TypeNode *pointedType, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(Pointer{pointedType}) {}

  std::string name() const; // rendered name for in-line printing

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

  void setInfo(Meta::TypeInfo *i) {
    assert(!mInfo);
    mInfo = i;
  }

  Meta::TypeInfo *info() { return mInfo; }

private:
  void doDump(int indent) const override;
  std::variant<Pointer, Named> mData;
  Meta::TypeInfo *mInfo = nullptr;
};

} // namespace Cougar::Ast