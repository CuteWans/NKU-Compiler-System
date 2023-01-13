#ifndef __TYPE_H__
#define __TYPE_H__
#include <string>
#include <vector>

class Type {
private:
  int kind;

protected:
  enum {
    INT,
    FLOAT,
    VOID,
    CONSTINT,
    CONSTFLOAT,
    INT_ARRAY,
    FLOAT_ARRAY,
    CONSTINT_ARRAY,
    CONSTFLOAT_ARRAY,
    ARRAY,
    FUNC,
    PTR,
    BOOL
  };
  int size;

public:
  Type(int kind, int size = 0) : kind(kind), size(size) {};
  virtual ~Type() {};
  virtual std::string toStr() = 0;

  int getType() const {
    return kind;
  }

  bool isInt() const {
    return kind == INT;
  }

  bool isVoid() const {
    return kind == VOID;
  }

  bool isFunc() const {
    return kind == FUNC;
  }

  bool isBool() const {
    return kind == BOOL;
  }

  bool isFloat() const {
    return kind == FLOAT;
  }

  bool isConstInt() const {
    return kind == CONSTINT;
  }

  bool isConstFloat() const {
    return kind == CONSTFLOAT;
  }

  bool isIntArray() const {
    return kind == INT_ARRAY;
  }

  bool isFloatArray() const {
    return kind == FLOAT_ARRAY;
  }

  bool isConstIntArray() const {
    return kind == CONSTINT_ARRAY;
  }

  bool isConstFloatArray() const {
    return kind == CONSTFLOAT_ARRAY;
  }

  bool isValue() const {
    return kind == INT || kind == FLOAT || kind == CONSTINT ||
      kind == CONSTFLOAT;
  }

  bool isArray() const {
    return kind == INT_ARRAY || kind == FLOAT_ARRAY || kind == CONSTINT_ARRAY ||
      kind == CONSTFLOAT_ARRAY;
  }

  bool isTypeInt() const {
    return kind == INT || kind == CONSTINT;
  }

  bool isTypeFloat() const {
    return kind == FLOAT || kind == CONSTFLOAT;
  }

  bool isTypeIntArray() const {
    return kind == INT_ARRAY || kind == CONSTINT_ARRAY;
  }

  bool isTypeFloatArray() const {
    return kind == FLOAT_ARRAY || kind == CONSTFLOAT_ARRAY;
  }
  int getSize() const { 
    return size; 
  };
  bool isPtr() const { 
    return kind == PTR; 
  }
};

class IntType : public Type {
private:
  int size;

public:
  IntType(int size) : Type(Type::INT), size(size) {};
  std::string toStr();
};

class BoolType : public Type {
private:
  int size;

public:
  BoolType(int size) : Type(Type::BOOL), size(size) {};
  std::string toStr();
};

class VoidType : public Type {
public:
  VoidType() : Type(Type::VOID) {};
  std::string toStr();
};

class FloatType : public Type {
private:
  int size;

public:
  FloatType(int size) : Type(Type::FLOAT), size(size) {};
  std::string toStr();
};

class ConstIntType : public Type {
public:
  ConstIntType() : Type(Type::CONSTINT) {};
  std::string toStr();
};

class ConstFloatType : public Type {
public:
  ConstFloatType() : Type(Type::CONSTFLOAT) {};
  std::string toStr();
};

class IntArrayType : public Type {
public:
  IntArrayType() : Type(Type::INT_ARRAY) {};
  std::string toStr();
};

class FloatArrayType : public Type {
public:
  FloatArrayType() : Type(Type::FLOAT_ARRAY) {};
  std::string toStr();
};

class ConstIntArrayType : public Type {
public:
  ConstIntArrayType() : Type(Type::CONSTINT_ARRAY) {};
  std::string toStr();
};

class ConstFloatArrayType : public Type {
public:
  ConstFloatArrayType() : Type(Type::CONSTFLOAT_ARRAY) {};
  std::string toStr();
};

class FuncType : public Type {
private:
  Type* returnType;
  std::vector<Type*> paramsType;

public:
  FuncType(Type* returnType, std::vector<Type*> paramsType) :
    Type(Type::FUNC), returnType(returnType), paramsType(paramsType) {};
  void setParamsType(std::vector<Type*> paramsType);

  Type* getRetType() {
    return returnType;
  }

  std::vector<Type*> getParamsType() {
    return paramsType;
  }

  std::string toStr();
};

class ArrayType : public Type {
  private:
  Type* elementType;
  Type* arrayType = nullptr;
  int length;
  bool constant;

  public:
  ArrayType(Type* elementType, int length, bool constant = false)
      : Type(Type::ARRAY),
        elementType(elementType),
        length(length),
        constant(constant) {
      size = elementType->getSize() * length;
  };
  std::string toStr();
  int getLength() const { return length; };
  Type* getElementType() const { return elementType; };
  void setArrayType(Type* arrayType) { this->arrayType = arrayType; };
  bool isConst() const { return constant; };
  Type* getArrayType() const { return arrayType; };
};

class PointerType : public Type {
private:
  Type* valueType;

public:
  PointerType(Type* valueType) : Type(Type::PTR) {
    this->valueType = valueType;
  }

  std::string toStr();
  Type* getType() const { return valueType; };
};

class TypeSys {
private:
  static IntType commonInt;
  static BoolType commonBool;
  static VoidType commonVoid;
  static FloatType commonFloat;
  static ConstIntType commonConstInt;
  static ConstFloatType commonConstFloat;
  static IntArrayType commonIntArray;
  static FloatArrayType commonFloatArray;
  static ConstIntArrayType commonConstIntArray;
  static ConstFloatArrayType commonConstFloatArray;

public:
  static Type* intType;
  static Type* voidType;
  static Type* boolType;
  static Type* floatType;
  static Type* constIntType;
  static Type* constFloatType;
  static Type* intArrayType;
  static Type* floatArrayType;
  static Type* constIntArrayType;
  static Type* constFloatArrayType;
};

#endif
