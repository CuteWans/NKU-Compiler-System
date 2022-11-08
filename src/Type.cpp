#include <assert.h>

#include <iostream>
#include <sstream>

#include "Type.h"

IntType   TypeSystem::commonInt      = IntType(4);
FloatType TypeSystem::commonFloat    = FloatType();
IntType   TypeSystem::commonConstInt = IntType(4, true);
VoidType  TypeSystem::commonVoid     = VoidType();

Type* TypeSystem::intType      = &commonInt;
Type* TypeSystem::floatType    = &commonFloat;
Type* TypeSystem::constIntType = &commonConstInt;
Type* TypeSystem::voidType     = &commonVoid;

std::string IntType::toStr() {
  if (constant) return "const int";
  else return "int";
}

std::string FloatType::toStr() {
  return "float";
}

std::string VoidType::toStr() {
  return "void";
}

std::string FunctionType::toStr() {
  std::ostringstream buffer;
  buffer << returnType->toStr() << "(";
  bool firstIdx = true;
  for (const auto& param : paramsType) {
    if (firstIdx) {
      buffer << param->toStr();
      firstIdx = false;
    } else buffer << ", " << param->toStr();
  }
  buffer << ')';
  return buffer.str();
}
