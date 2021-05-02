#pragma once

#include "node.hh"

namespace Cougar::Meta {
class FunctionInfo;
}

namespace Cougar::Ast {

class TypeNode;
class Statement;

struct FunctionArg {
public:
  FunctionArg(TypeNode *argType, std::string_view argName)
      : type(argType), name(argName) {
    assert(argType);
  }

  void dump(int indent = 0) const;

  TypeNode *type = nullptr;
  std::string_view name;
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

  void setBody(Statement *body);
  Statement *body() { return mBody; }
  TypeNode *returnType() { return mReturnType; }

  std::string_view name() const { return mName; }

  Utils::ListView<FunctionArg> args() { return mArgs; }

  void setInfo(Meta::FunctionInfo *fi) {
    assert(!mInfo);
    mInfo = fi;
  }
  Meta::FunctionInfo *info() { return mInfo; }

private:
  void doDump(int indent = 0) const override;

  Access mAccess;
  std::string_view mName;
  TypeNode *mReturnType = nullptr;
  Statement *mBody = nullptr;

  Utils::List<FunctionArg> mArgs;
  Meta::FunctionInfo *mInfo = nullptr;
};

} // namespace Cougar::Ast