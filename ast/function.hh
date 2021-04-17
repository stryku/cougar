#pragma once

#include "node.hh"

namespace Cougar::Ast {

// TODO move to own file
class TypeName : public NodeOnToken {
public:
  TypeName(std::string_view name, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mTypeName(name) {}

  std::string_view typeName() const { return mTypeName; }

private:
  void doDump(int indent = 0) const override;

  std::string_view mTypeName;
  const Lexer::Token *mToken;
};

struct FunctionArg : Node {
public:
  FunctionArg(TypeName *argType, std::string_view argName)
      : mType(argType), mName(argName) {
    assert(argType);
  }

private:
  void doDump(int indent = 0) const override;

  TypeName *mType;
  std::string_view mName;
};

class FunctionDeclaration : public NodeOnToken {
public:
  FunctionDeclaration(Access access, std::string_view name,
                      TypeName *returnType, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mAccess(access), mName(name),
        mReturnType(returnType) {
    assert(returnType);
  }

  void addArg(TypeName *argType, std::string_view argName);

private:
  void doDump(int indent = 0) const override;

  Access mAccess;
  std::string_view mName;
  TypeName *mReturnType;

  Utils::List<FunctionArg> mArgs;
};

} // namespace Cougar::Ast