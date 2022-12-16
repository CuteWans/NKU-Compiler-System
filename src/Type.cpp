#include <sstream>
#include "Type.h"

IntType TypeSys::commonInt = IntType(32);
BoolType TypeSys::commonBool = BoolType(1);
VoidType TypeSys::commonVoid = VoidType();
FloatType TypeSys::commonFloat = FloatType(4);
ConstIntType TypeSys::commonConstInt = ConstIntType();
ConstFloatType TypeSys::commonConstFloat = ConstFloatType();
IntArrayType TypeSys::commonIntArray = IntArrayType();
FloatArrayType TypeSys::commonFloatArray = FloatArrayType();
ConstIntArrayType TypeSys::commonConstIntArray = ConstIntArrayType();
ConstFloatArrayType TypeSys::commonConstFloatArray = ConstFloatArrayType();

Type* TypeSys::intType = &commonInt;
Type* TypeSys::voidType = &commonVoid;
Type* TypeSys::boolType = &commonBool;
Type* TypeSys::floatType = &commonFloat;
Type* TypeSys::constIntType = &commonConstInt;
Type* TypeSys::constFloatType = &commonConstFloat;
Type* TypeSys::intArrayType = &commonIntArray;
Type* TypeSys::floatArrayType = &commonFloatArray;
Type* TypeSys::constIntArrayType = &commonConstIntArray;
Type* TypeSys::constFloatArrayType = &commonConstFloatArray;

std::string IntType::toStr() {
  std::ostringstream buffer;
  buffer << "i" << size;
  return buffer.str();
}

std::string BoolType::toStr() {
  std::ostringstream buffer;
  buffer << "i" << size;
  return buffer.str();
}

std::string VoidType::toStr() {
  return "void";
}

std::string FloatType::toStr() {
  return "float";
}

std::string ConstIntType::toStr() {
  return "constant int";
}

std::string ConstFloatType::toStr() {
  return "constant float";
}

std::string IntArrayType::toStr() {
  return "int array";
}

std::string FloatArrayType::toStr() {
  return "float array";
}

std::string ConstIntArrayType::toStr() {
  return "const int array";
}

std::string ConstFloatArrayType::toStr() {
  return "const float array";
}

void FuncType::setParamsType(std::vector<Type*> paramsType) {
  this->paramsType = paramsType;
}

std::string FuncType::toStr() {
  return returnType->toStr();
}

std::string PointerType::toStr() {
  std::ostringstream buffer;
  buffer << valueType->toStr() << "*";
  return buffer.str();
}
