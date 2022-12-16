#include <iostream>
#include <sstream>
#include "SymbolTable.h"
#include "Type.h"
extern FILE* yyout;

SymbEntry::SymbEntry(Type* type, int kind) {
  this->type = type;
  this->kind = kind;
}

ConstSymbEntry::ConstSymbEntry(Type* type, int value) :
  SymbEntry(type, SymbEntry::CONSTANT) {
  this->value = value;
}

std::string ConstSymbEntry::toStr() {
  std::ostringstream buffer;
  buffer << value;
  return buffer.str();
}

IdSymbEntry::IdSymbEntry(Type* type, std::string name, int scope) :
  SymbEntry(type, SymbEntry::VARIABLE), name(name) {
  this->scope = scope;
  addr = nullptr;
}

std::string IdSymbEntry::toStr() {
  return "@" + name;
}

bool IdSymbEntry::isLibFunc() const {
  if (name == "getint" || name == "putint" || name == "putch") return true;
  return false;
}

void IdSymbEntry::output() const {
  if (type->isFunc()) {
    fprintf(yyout, "declare %s @%s(",
      ((FuncType*) type)->getRetType()->toStr().c_str(), name.c_str());
    auto paramsType = ((FuncType*) type)->getParamsType();
    auto paramType = paramsType.begin();
    if (paramType != paramsType.end()) {
      fprintf(yyout, "%s", (*paramType)->toStr().c_str());
      ++paramType;
      for (; paramType != paramsType.end(); ++paramType) {
        fprintf(yyout, ",%s", (*paramType)->toStr().c_str());
      }
    }
    fprintf(yyout, ")\n");
  } else {
    //assert(scope==GLOBAL);
    fprintf(yyout, "@%s = dso_local global %s %d\n", name.c_str(),
      type->toStr().c_str(), (int) value);
  }
}

TempSymbEntry::TempSymbEntry(Type* type, int label) :
  SymbEntry(type, SymbEntry::TEMPORARY) {
  this->label = label;
}

std::string TempSymbEntry::toStr() {
  std::ostringstream buffer;
  buffer << "%t" << label;
  return buffer.str();
}

SymbTable::SymbTable() {
  prev = nullptr;
  level = 0;
  Type* func_getint = new FuncType(TypeSys::intType, {});
  SymbEntry* se_getint = new IdSymbEntry(func_getint, "getint", 0);
  this->install("getint", se_getint);

  Type* func_putint = new FuncType(TypeSys::voidType, {TypeSys::intType});
  SymbEntry* se_putint = new IdSymbEntry(func_putint, "putint", 0);
  this->install("putint", se_putint);

  Type* func_putch = new FuncType(TypeSys::voidType, {TypeSys::intType});
  SymbEntry* se_putch = new IdSymbEntry(func_putch, "putch", 0);
  this->install("putch", se_putch);
}

SymbTable::SymbTable(SymbTable* prev) {
  this->prev = prev;
  this->level = prev->level + 1;
}

/*
    Description: lookup the symbol entry of an identifier in the symbol table
    Parameters: 
        name: identifier name
    Return: pointer to the symbol entry of the identifier

    hint:
    1. The symbol table is a stack. The top of the stack contains symbol entries in the current scope.
    2. Search the entry in the current symbol table at first.
    3. If it's not in the current table, search it in previous ones(along the 'prev' link).
    4. If you find the entry, return it.
    5. If you can't find it in all symbol tables, return nullptr.
*/
SymbEntry* SymbTable::lookup(std::string name) {
  // Todo
  auto it = symbolTable.find(name);
  if (it != symbolTable.end()) {
    return it->second;
  } else if (prev != nullptr) {
    return prev->lookup(name);
  } else {
    return nullptr;
  }
}

SymbEntry* SymbTable::lookupCur(std::string name) {
  // Todo
  auto it = symbolTable.find(name);
  if (it != symbolTable.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

// install the entry into current symbol table.
void SymbTable::install(std::string name, SymbEntry* entry) {
  symbolTable[name] = entry;
}

int SymbTable::counter = 0;
static SymbTable t;
SymbTable* identifiers = &t;
SymbTable* globals = &t;
