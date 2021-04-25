#pragma once

#include "node.hh"

namespace Cougar::Ast {

class TypeNode;
class StatementGroup;

struct FunctionArg : Node {
public:
  FunctionArg(TypeNode *argType, std::string_view argName)
      : mType(argType), mName(argName) {
    assert(argType);
  }

private:
  void doDump(int indent = 0) const override;

  TypeNode *mType;
  std::string_view mName;
};

class FunctionDeclaration : public NodeOnToken {
public:
  FunctionDeclaration(Access access, std::string_view name,
                      TypeNode *returnType, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mAccess(access), mName(name),
        mReturnType(returnType) {
    assert(returnType);
  }

  void addArg(TypeNode *argType, std::string_view argName);

  void setBody(StatementGroup *body);
  StatementGroup *body() { return mBody; }
  TypeNode *returnType() { return mReturnType; }

private:
  void doDump(int indent = 0) const override;

  Access mAccess;
  std::string_view mName;
  TypeNode *mReturnType;
  StatementGroup *mBody;

  Utils::List<FunctionArg> mArgs;
};

} // namespace Cougar::Ast