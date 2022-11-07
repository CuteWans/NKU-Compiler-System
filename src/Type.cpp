#include <sstream>

#include "Type.h"

IntType   TypeSystem::commonInt   = IntType(4);
FloatType TypeSystem::commonFloat = FloatType();
VoidType  TypeSystem::commonVoid  = VoidType();

Type* TypeSystem::intType   = &commonInt;
Type* TypeSystem::voidType  = &commonVoid;
Type* TypeSystem::floatType = &commonFloat;

std::string IntType::toStr() {
  return "int";
}

std::string FloatType::toStr() {
  return "float";
}

std::string VoidType::toStr() {
  return "void";
}

std::string FunctionType::toStr() {
  std::ostringstream buffer;
  buffer << returnType->toStr() << "()";
  return buffer.str();
}
