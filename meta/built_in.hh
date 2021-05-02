#pragma once

namespace Cougar::Meta {

class Scope;
class TypeInfo;

// TODO this should be in a different library
Scope *createBuiltInScope();

namespace BuiltIn {

extern TypeInfo *typeCStr;
extern TypeInfo *typeInt8;
extern TypeInfo *typeInt16;
extern TypeInfo *typeInt32;
extern TypeInfo *typeInt64;

} // namespace BuiltIn

} // namespace Cougar::Meta