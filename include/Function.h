#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "BasicBlock.h"
#include "SymbolTable.h"

class Unit;

class Func {
  typedef std::vector<BasicBlock*>::iterator iterator;
  typedef std::vector<BasicBlock*>::reverse_iterator reverse_iterator;

private:
  std::vector<BasicBlock*> block_list;
  std::vector<Op*> params;
  SymbEntry* sym_ptr;
  BasicBlock* entry;
  Unit* parent;

public:
  Func(Unit*, SymbEntry*);
  ~Func();

  void insertBlock(BasicBlock* bb) {
    block_list.push_back(bb);
  }

  BasicBlock* getEntry() {
    return entry;
  }

  void insertParam(Op* pa) {
    params.push_back(pa);
  }

  void remove(BasicBlock* bb);
  void output() const;

  std::vector<BasicBlock*>& getBlockList() {
    return block_list;
  }

  iterator begin() {
    return block_list.begin();
  }

  iterator end() {
    return block_list.end();
  }

  reverse_iterator rbegin() {
    return block_list.rbegin();
  }

  reverse_iterator rend() {
    return block_list.rend();
  }

  SymbEntry* getSymPtr() {
    return sym_ptr;
  }
  void genMachineCode(AsmBuilder*);
};

#endif
