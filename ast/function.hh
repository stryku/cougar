#pragma once

#include "node.hh"

namespace Cougar::Ast {

// TODO move to own file
class Type : public NodeOnToken {
public:
  Type(std::string_view name, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mTypeName(name) {}

  std::string_view typeName() const { return mTypeName; }

private:
  void doDump(int indent = 0) const override;

  std::string_view mTypeName;
  const Lexer::Token *mToken;
};

struct FunctionArg : Node {
public:
  FunctionArg(Type *argType, std::string_view argName)
      : mType(argType), mName(argName) {
    assert(argType);
  }

private:
  void doDump(int indent = 0) const override;

  Type *mType;
  std::string_view mName;
};

class FunctionDeclaration : public NodeOnToken {
public:
  FunctionDeclaration(Access access, std::string_view name, Type *returnType,
                      const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mAccess(access), mName(name),
        mReturnType(returnType) {
    assert(returnType);
  }

  void addArg(Utils::ZoneAllocator &zone, Type *argType,
              std::string_view argName);

private:
  void doDump(int indent = 0) const override;

  Access mAccess;
  std::string_view mName;
  Type *mReturnType;

  Utils::List<FunctionArg> mArgs;
};

} // namespace Cougar::Ast