#include <iostream>
#include "BasicBlock.h"
#include "Function.h"
#include "Instruction.h"
#include "Type.h"
extern FILE* yyout;

Inst::Inst(unsigned instType, BasicBlock* insert_bb) {
  prev = next = this;
  opcode = -1;
  this->instType = instType;
  if (insert_bb != nullptr) {
    insert_bb->insertBack(this);
    parent = insert_bb;
  }
}

Inst::~Inst() {
  parent->remove(this);
}

BasicBlock* Inst::getParent() {
  return parent;
}

void Inst::setParent(BasicBlock* bb) {
  parent = bb;
}

void Inst::setNext(Inst* inst) {
  next = inst;
}

void Inst::setPrev(Inst* inst) {
  prev = inst;
}

Inst* Inst::getNext() {
  return next;
}

Inst* Inst::getPrev() {
  return prev;
}

BinInst::BinInst(
  unsigned opcode, Op* dst, Op* src1, Op* src2, BasicBlock* insert_bb) :
  Inst(BINARY, insert_bb) {
  this->opcode = opcode;
  operands.push_back(dst);
  operands.push_back(src1);
  operands.push_back(src2);
  dst->setDef(this);
  src1->addUse(this);
  src2->addUse(this);
}

BinInst::~BinInst() {
  operands[0]->setDef(nullptr);
  if (operands[0]->usersNum() == 0) delete operands[0];
  operands[1]->removeUse(this);
  operands[2]->removeUse(this);
}

void BinInst::output() const {
  std::string s1, s2, s3, op, type;
  s1 = operands[0]->toStr();
  s2 = operands[1]->toStr();
  s3 = operands[2]->toStr();
  type = operands[0]->getType()->toStr();
  switch (opcode) {
    case ADD : op = "add"; break;
    case SUB : op = "sub"; break;
    case MUL : op = "mul"; break;
    case DIV : op = "sdiv"; break;
    case MOD : op = "srem"; break;
    default : break;
  }
  fprintf(yyout, "  %s = %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(),
    s2.c_str(), s3.c_str());
}

CmpInst::CmpInst(
  unsigned opcode, Op* dst, Op* src1, Op* src2, BasicBlock* insert_bb) :
  Inst(CMP, insert_bb) {
  this->opcode = opcode;
  operands.push_back(dst);
  operands.push_back(src1);
  operands.push_back(src2);
  dst->setDef(this);
  src1->addUse(this);
  src2->addUse(this);
}

CmpInst::~CmpInst() {
  operands[0]->setDef(nullptr);
  if (operands[0]->usersNum() == 0) delete operands[0];
  operands[1]->removeUse(this);
  operands[2]->removeUse(this);
}

void CmpInst::output() const {
  std::string s1, s2, s3, op, type;
  s1 = operands[0]->toStr();
  s2 = operands[1]->toStr();
  s3 = operands[2]->toStr();
  type = operands[1]->getType()->toStr();
  switch (opcode) {
    case E : op = "eq"; break;
    case NE : op = "ne"; break;
    case L : op = "slt"; break;
    case LE : op = "sle"; break;
    case G : op = "sgt"; break;
    case GE : op = "sge"; break;
    default : op = ""; break;
  }

  fprintf(yyout, "  %s = icmp %s %s %s, %s\n", s1.c_str(), op.c_str(),
    type.c_str(), s2.c_str(), s3.c_str());
}

UncondBrInst::UncondBrInst(BasicBlock* to, BasicBlock* insert_bb) :
  Inst(UNCOND, insert_bb) {
  branch = to;
}

void UncondBrInst::output() const {
  fprintf(yyout, "  br label %%B%d\n", branch->getNo());
}

void UncondBrInst::setBranch(BasicBlock* bb) {
  branch = bb;
}

BasicBlock* UncondBrInst::getBranch() {
  return branch;
}

CondBrInst::CondBrInst(BasicBlock* true_branch, BasicBlock* false_branch,
  Op* cond, BasicBlock* insert_bb) :
  Inst(COND, insert_bb) {
  this->true_branch = true_branch;
  this->false_branch = false_branch;
  cond->addUse(this);
  operands.push_back(cond);
}

CondBrInst::~CondBrInst() {
  operands[0]->removeUse(this);
}

void CondBrInst::output() const {
  std::string cond, type;
  cond = operands[0]->toStr();
  type = operands[0]->getType()->toStr();
  int true_label = true_branch->getNo();
  int false_label = false_branch->getNo();
  fprintf(yyout, "  br %s %s, label %%B%d, label %%B%d\n", type.c_str(),
    cond.c_str(), true_label, false_label);
}

void CondBrInst::setFalseBranch(BasicBlock* bb) {
  false_branch = bb;
}

BasicBlock* CondBrInst::getFalseBranch() {
  return false_branch;
}

void CondBrInst::setTrueBranch(BasicBlock* bb) {
  true_branch = bb;
}

BasicBlock* CondBrInst::getTrueBranch() {
  return true_branch;
}

RetInst::RetInst(Op* src, BasicBlock* insert_bb) : Inst(RET, insert_bb) {
  if (src != nullptr) {
    operands.push_back(src);
    src->addUse(this);
  }
}

RetInst::~RetInst() {
  if (!operands.empty()) operands[0]->removeUse(this);
}

void RetInst::output() const {
  if (operands.empty()) {
    fprintf(yyout, "  ret void\n");
  } else {
    std::string ret, type;
    ret = operands[0]->toStr();
    type = operands[0]->getType()->toStr();
    fprintf(yyout, "  ret %s %s\n", type.c_str(), ret.c_str());
  }
}

AllocaInst::AllocaInst(Op* dst, SymbEntry* se, BasicBlock* insert_bb) :
  Inst(ALLOCA, insert_bb) {
  operands.push_back(dst);
  dst->setDef(this);
  this->se = se;
}

AllocaInst::~AllocaInst() {
  operands[0]->setDef(nullptr);
  if (operands[0]->usersNum() == 0) delete operands[0];
}

void AllocaInst::output() const {
  std::string dst, type;
  dst = operands[0]->toStr();
  type = se->getType()->toStr();
  fprintf(yyout, "  %s = alloca %s, align 4\n", dst.c_str(), type.c_str());
}

LoadInst::LoadInst(Op* dst, Op* src_addr, BasicBlock* insert_bb) :
  Inst(LOAD, insert_bb) {
  operands.push_back(dst);
  operands.push_back(src_addr);
  dst->setDef(this);
  src_addr->addUse(this);
}

LoadInst::~LoadInst() {
  operands[0]->setDef(nullptr);
  if (operands[0]->usersNum() == 0) delete operands[0];
  operands[1]->removeUse(this);
}

void LoadInst::output() const {
  std::string dst = operands[0]->toStr();
  std::string src = operands[1]->toStr();
  std::string src_type;
  std::string dst_type;
  dst_type = operands[0]->getType()->toStr();
  src_type = operands[1]->getType()->toStr();
  fprintf(yyout, "  %s = load %s, %s %s, align 4\n", dst.c_str(),
    dst_type.c_str(), src_type.c_str(), src.c_str());
}

StoreInst::StoreInst(Op* dst_addr, Op* src, BasicBlock* insert_bb) :
  Inst(STORE, insert_bb) {
  operands.push_back(dst_addr);
  operands.push_back(src);
  dst_addr->addUse(this);
  src->addUse(this);
}

StoreInst::~StoreInst() {
  operands[0]->removeUse(this);
  operands[1]->removeUse(this);
}

void StoreInst::output() const {
  std::string dst = operands[0]->toStr();
  std::string src = operands[1]->toStr();
  std::string dst_type = operands[0]->getType()->toStr();
  std::string src_type = operands[1]->getType()->toStr();

  fprintf(yyout, "  store %s %s, %s %s, align 4\n", src_type.c_str(),
    src.c_str(), dst_type.c_str(), dst.c_str());
}

CallInst::CallInst(
  Op* dst, SymbEntry* se, std::vector<Op*> params, BasicBlock* insert_bb) :
  Inst(CALL, insert_bb) {
  this->se = se;
  operands.push_back(dst);
  dst->setDef(this);
  for (const auto& param : params) {
    operands.push_back(param);
    param->addUse(this);
  }
}

CallInst::~CallInst() {
  operands[0]->setDef(nullptr);
  if (operands[0]->usersNum() == 0) delete operands[0];
  for (const auto& i : operands) i->removeUse(this);
}

void CallInst::output() const {
  bool isVoid = ((FuncType*) (operands[0]->getType()))->isVoid();
  if (isVoid) {
    fprintf(yyout, "  ");
  } else {
    fprintf(yyout, "  %s = ", operands[0]->toStr().c_str());
  }
  fprintf(yyout, "call %s %s(",
    ((FuncType*) se->getType())->getRetType()->toStr().c_str(),
    se->toStr().c_str());
  for (size_t i = 1; i < operands.size(); ++i) {
    if (i >= 2) fprintf(yyout, ", ");
    fprintf(yyout, "%s %s", operands[i]->getType()->toStr().c_str(),
      operands[i]->toStr().c_str());
  }
  fprintf(yyout, ")\n");
}

XorInst::XorInst(Op* dst, Op* src, BasicBlock* insert_bb) :
  Inst(XOR, insert_bb) {
  operands.push_back(dst);
  operands.push_back(src);
  dst->setDef(this);
  src->addUse(this);
}

void XorInst::output() const {
  fprintf(yyout, "  %s = xor %s %s, true\n", operands[0]->toStr().c_str(),
    operands[1]->getType()->toStr().c_str(), operands[1]->toStr().c_str());
}

XorInst::~XorInst() {
  operands[0]->setDef(nullptr);
  if (operands[0]->usersNum() == 0) delete operands[0];
  operands[1]->removeUse(this);
}

ZextInst::ZextInst(Op* dst, Op* src, BasicBlock* insert_bb) :
  Inst(ZEXT, insert_bb) {
  operands.push_back(dst);
  operands.push_back(src);
  dst->setDef(this);
  src->addUse(this);
}

void ZextInst::output() const {
  fprintf(yyout, "  %s = zext %s %s to i32\n", operands[0]->toStr().c_str(),
    operands[1]->getType()->toStr().c_str(), operands[1]->toStr().c_str());
}

ZextInst::~ZextInst() {
  operands[0]->setDef(nullptr);
  if (operands[0]->usersNum() == 0) delete operands[0];
  operands[1]->removeUse(this);
}