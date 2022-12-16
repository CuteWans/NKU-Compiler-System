#ifndef __OPERAND_H__
#define __OPERAND_H__

#include <vector>
#include "SymbolTable.h"

class Inst;
class Func;

// class Op - The operand of an instruction.
class Op {
  typedef std::vector<Inst*>::iterator use_iterator;

private:
  Inst* def;                // The instruction where this operand is defined.
  std::vector<Inst*> uses;  // Intructions that use this operand.
  SymbEntry* se;            // The symbol entry of this operand.

public:
  Op(SymbEntry* se) : se(se) {
    def = nullptr;
  }

  void setDef(Inst* inst) {
    def = inst;
  }

  void addUse(Inst* inst) {
    uses.push_back(inst);
  }

  void removeUse(Inst* inst);

  int usersNum() const {
    return uses.size();
  }

  use_iterator use_begin() {
    return uses.begin();
  }

  use_iterator use_end() {
    return uses.end();
  }

  Type* getType() {
    return se->getType();
  }

  std::string toStr() const;
};

#endif