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

  bool isAlloc() const { 
    return instType == ALLOCA; 
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
  MachineOperand* genMachineOperand(Op*);
  MachineOperand* genMachineReg(int reg);
  MachineOperand* genMachineVReg();
  MachineOperand* genMachineImm(int val);
  MachineOperand* genMachineLabel(int block_no);
  virtual void genMachineCode(AsmBuilder*) = 0;

protected:
  unsigned instType;
  unsigned opcode;
  Inst* prev;
  Inst* next;
  BasicBlock* parent;
  std::vector<Op*> operands;

  enum { BINARY, COND, UNCOND, RET, LOAD, STORE, CMP, ALLOCA, CALL, XOR, ZEXT, GEP };
};

// meaningless instruction, used as the head node of the instruction list.
class DummyInst : public Inst {
public:
  DummyInst() : Inst(-1, nullptr) {};
  void output() const {};
  void genMachineCode(AsmBuilder*) {};
};

class AllocaInst : public Inst {
public:
  AllocaInst(Op* dst, SymbEntry* se, BasicBlock* insert_bb = nullptr);
  ~AllocaInst();
  void output() const;
  void genMachineCode(AsmBuilder*);

private:
  SymbEntry* se;
};

class LoadInst : public Inst {
public:
  LoadInst(Op* dst, Op* src_addr, BasicBlock* insert_bb = nullptr);
  ~LoadInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
};

class StoreInst : public Inst {
public:
  StoreInst(Op* dst_addr, Op* src, BasicBlock* insert_bb = nullptr);
  ~StoreInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
};

class BinInst : public Inst {
public:
  BinInst(unsigned opcode, Op* dst, Op* src1, Op* src2,
    BasicBlock* insert_bb = nullptr);
  ~BinInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
  enum { SUB, ADD, MUL, DIV, MOD, AND, OR };
};

class CmpInst : public Inst {
public:
  CmpInst(unsigned opcode, Op* dst, Op* src1, Op* src2,
    BasicBlock* insert_bb = nullptr);
  ~CmpInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
  enum { E, NE, L, GE, G, LE };
};

// unconditional branch
class UncondBrInst : public Inst {
public:
  UncondBrInst(BasicBlock*, BasicBlock* insert_bb = nullptr);
  void output() const;
  void setBranch(BasicBlock*);
  BasicBlock* getBranch();
  void genMachineCode(AsmBuilder*);
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
  void genMachineCode(AsmBuilder*);
protected:
  BasicBlock* true_branch;
  BasicBlock* false_branch;
};

class RetInst : public Inst {
public:
  RetInst(Op* src, BasicBlock* insert_bb = nullptr);
  ~RetInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
};

class CallInst : public Inst {
private:
  SymbEntry* se;
  Op* dst;

public:
  CallInst(Op* dst, SymbEntry* se, std::vector<Op*> params,
    BasicBlock* insert_bb = nullptr);
  ~CallInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
};

class XorInst : public Inst {
public:
  XorInst(Op* dst, Op* src, BasicBlock* insert_bb = nullptr);
  ~XorInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
};

class ZextInst : public Inst {
public:
  ZextInst(Op* dst, Op* src, BasicBlock* insert_bb = nullptr);
  ~ZextInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
};

class GepInst : public Inst {
private:
  bool paramFirst;
  bool first;
  bool last;
  Op* init;

public:
  GepInst(Op* dst,
                  Op* arr,
                  Op* idx,
                  BasicBlock* insert_bb = nullptr,
                  bool paramFirst = false);
  ~GepInst();
  void output() const;
  void genMachineCode(AsmBuilder*);
  void setFirst() { first = true; };
  void setLast() { last = true; };
  Op* getInit() const { return init; };
  void setInit(Op* init) { this->init = init; }; 
};

#endif