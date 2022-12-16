#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__
#include <set>
#include <vector>
#include "Instruction.h"

class Func;

class BasicBlock {
  typedef std::vector<BasicBlock*>::iterator bb_iterator;

private:
  std::vector<BasicBlock*> pred, succ;
  Inst* head;
  Func* parent;
  int no;

public:
  BasicBlock(Func*);
  ~BasicBlock();
  void insertFront(Inst*);
  void insertBack(Inst*);
  void insertBefore(Inst*, Inst*);
  void remove(Inst*);

  bool empty() const {
    return head->getNext() == head;
  }

  void output() const;

  bool succEmpty() const {
    return succ.empty();
  }

  bool predEmpty() const {
    return pred.empty();
  }

  void addSucc(BasicBlock*);
  void removeSucc(BasicBlock*);
  void addPred(BasicBlock*);
  void removePred(BasicBlock*);

  int getNo() {
    return no;
  }

  Func* getParent() {
    return parent;
  }

  Inst* begin() {
    return head->getNext();
  }

  Inst* end() {
    return head;
  }

  Inst* rbegin() {
    return head->getPrev();
  }

  Inst* rend() {
    return head;
  }

  bb_iterator succ_begin() {
    return succ.begin();
  }

  bb_iterator succ_end() {
    return succ.end();
  }

  bb_iterator pred_begin() {
    return pred.begin();
  }

  bb_iterator pred_end() {
    return pred.end();
  }

  int getNumOfPred() const {
    return pred.size();
  }

  int getNumOfSucc() const {
    return succ.size();
  }
};

#endif