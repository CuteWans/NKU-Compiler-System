#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <map>
#include <vector>
#include "Operand.h"

class BasicBlock;

class Inst {
public:
  Inst(unsigned instType, BasicBlock* insert_bb = nullptr);
  virtual ~Inst();
  BasicBlock* getParent();

  bool isUncond() const {
    return instType == UNCOND;
  }

  bool isCond() const {
    return instType == COND;
  }

  bool isRet() const {
    return instType == RET;
  }

  void setParent(BasicBlock*);
  void setNext(Inst*);
  void setPrev(Inst*);
  Inst* getNext();
  Inst* getPrev();
  virtual void output() const = 0;

protected:
  unsigned instType;
  unsigned opcode;
  Inst* prev;
  Inst* next;
  BasicBlock* parent;
  std::vector<Op*> operands;

  enum { BINARY, COND, UNCOND, RET, LOAD, STORE, CMP, ALLOCA, CALL, XOR, ZEXT };
};

// meaningless instruction, used as the head node of the instruction list.
class DummyInst : public Inst {
public:
  DummyInst() : Inst(-1, nullptr) {};
  void output() const {};
};

class AllocaInst : public Inst {
public:
  AllocaInst(Op* dst, SymbEntry* se, BasicBlock* insert_bb = nullptr);
  ~AllocaInst();
  void output() const;

private:
  SymbEntry* se;
};

class LoadInst : public Inst {
public:
  LoadInst(Op* dst, Op* src_addr, BasicBlock* insert_bb = nullptr);
  ~LoadInst();
  void output() const;
};

class StoreInst : public Inst {
public:
  StoreInst(Op* dst_addr, Op* src, BasicBlock* insert_bb = nullptr);
  ~StoreInst();
  void output() const;
};

class BinaryInst : public Inst {
public:
  BinaryInst(unsigned opcode, Op* dst, Op* src1, Op* src2,
    BasicBlock* insert_bb = nullptr);
  ~BinaryInst();
  void output() const;

  enum { SUB, ADD, MUL, DIV, MOD, AND, OR };
};

class CmpInst : public Inst {
public:
  CmpInst(unsigned opcode, Op* dst, Op* src1, Op* src2,
    BasicBlock* insert_bb = nullptr);
  ~CmpInst();
  void output() const;

  enum { E, NE, L, GE, G, LE };
};

// unconditional branch
class UncondBrInst : public Inst {
public:
  UncondBrInst(BasicBlock*, BasicBlock* insert_bb = nullptr);
  void output() const;
  void setBranch(BasicBlock*);
  BasicBlock* getBranch();

protected:
  BasicBlock* branch;
};

// conditional branch
class CondBrInst : public Inst {
public:
  CondBrInst(BasicBlock*, BasicBlock*, Op*, BasicBlock* insert_bb = nullptr);
  ~CondBrInst();
  void output() const;
  void setTrueBranch(BasicBlock*);
  BasicBlock* getTrueBranch();
  void setFalseBranch(BasicBlock*);
  BasicBlock* getFalseBranch();

protected:
  BasicBlock* true_branch;
  BasicBlock* false_branch;
};

class RetInst : public Inst {
public:
  RetInst(Op* src, BasicBlock* insert_bb = nullptr);
  ~RetInst();
  void output() const;
};

class CallInst : public Inst {
private:
  SymbEntry* se;

public:
  CallInst(Op* dst, SymbEntry* se, std::vector<Op*> params,
    BasicBlock* insert_bb = nullptr);
  ~CallInst();
  void output() const;
};

class XorInst : public Inst {
public:
  XorInst(Op* dst, Op* src, BasicBlock* insert_bb = nullptr);
  ~XorInst();
  void output() const;
};

class ZextInst : public Inst {
public:
  ZextInst(Op* dst, Op* src, BasicBlock* insert_bb = nullptr);
  ~ZextInst();
  void output() const;
};

#endif