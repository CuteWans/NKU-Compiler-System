#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <map>
#include <string>

class Type;
class Op;

class SymbEntry {
private:
  int kind;

protected:
  enum { CONSTANT, VARIABLE, TEMPORARY };

  Type* type;

public:
  SymbEntry(Type* type, int kind);
  virtual ~SymbEntry() {};

  bool isConst() const {
    return kind == CONSTANT;
  }

  bool isTemp() const {
    return kind == TEMPORARY;
  }

  bool isVariable() const {
    return kind == VARIABLE;
  }

  Type* getType() {
    return type;
  }

  void setType(Type* type) {
    this->type = type;
  }

  virtual std::string toStr() = 0;
  // You can add any function you need here.
};

/*  
    Symb entry for literal constant. Example:

    int a = 1;

    Compiler should create constant symbol entry for literal constant '1'.
*/
class ConstSymbEntry : public SymbEntry {
private:
  int value;

public:
  ConstSymbEntry(Type* type, int value);
  virtual ~ConstSymbEntry() {};

  int getValue() const {
    return value;
  }

  std::string toStr();
  // You can add any function you need here.
};

/* 
    Symb entry for identifier. Example:

    int a;
    int b;
    void f(int c)
    {
        int d;
        {
            int e;
        }
    }

    Compiler should create identifier symbol entries for variables a, b, c, d and e:

    | variable | scope    |
    | a        | GLOBAL   |
    | b        | GLOBAL   |
    | c        | PARAM    |
    | d        | LOCAL    |
    | e        | LOCAL +1 |
*/
class IdSymbEntry : public SymbEntry {
private:
  enum { GLOBAL, PARAM, LOCAL };

  std::string name;

  int scope;
  Op* addr;  // The address of the identifier.
  // used for global definition
  float value;
  bool isValue = false;
  // You can add any field you need here.

public:
  IdSymbEntry(Type* type, std::string name, int scope);
  virtual ~IdSymbEntry() {};
  std::string toStr();

  bool isGlobal() const {
    return scope == GLOBAL;
  }

  bool isParam() const {
    return scope == PARAM;
  }

  bool isLocal() const {
    return scope >= LOCAL;
  }

  bool isLibFunc() const;

  int getScope() const {
    return scope;
  }

  void setAddr(Op* addr) {
    this->addr = addr;
  }

  Op* getAddr() {
    return addr;
  }

  void setValue(float v) {
    value = v;
    isValue = true;
  }

  float getValue() {
    return value;
  }

  std::string getName() {
    return name;
  }

  void output() const;
  // You can add any function you need here.
};

/* 
    Symb entry for temporary variable created by compiler. Example:

    int a;
    a = 1 + 2 + 3;

    The compiler would generate intermediate code like:

    t1 = 1 + 2
    t2 = t1 + 3
    a = t2

    So compiler should create temporary symbol entries for t1 and t2:

    | temporary variable | label |
    | t1                 | 1     |
    | t2                 | 2     |
*/
class TempSymbEntry : public SymbEntry {
private:
  int label;

public:
  TempSymbEntry(Type* type, int label);
  virtual ~TempSymbEntry() {};
  std::string toStr();

  int getLabel() const {
    return label;
  }

  // You can add any function you need here.
};

// symbol table managing identifier symbol entries
class SymbTable {
private:
  std::map<std::string, SymbEntry*> symbolTable;
  SymbTable* prev;
  int level;
  static int counter;

public:
  SymbTable();
  SymbTable(SymbTable* prev);
  void install(std::string name, SymbEntry* entry);
  SymbEntry* lookup(std::string name);
  SymbEntry* lookupCur(std::string name);

  SymbTable* getPrev() {
    return prev;
  }

  int getLevel() {
    return level;
  }

  static int getLabel() {
    return counter++;
  }
};

extern SymbTable* identifiers;
extern SymbTable* globals;

#endif
