#include <iostream>
#include <sstream>
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

GepInst::GepInst(Op* dst,
                               Op* arr,
                               Op* idx,
                               BasicBlock* insert_bb,
                               bool paramFirst)
    : Inst(GEP, insert_bb), paramFirst(paramFirst) {
  operands.push_back(dst);
  operands.push_back(arr);
  operands.push_back(idx);
  dst->setDef(this);
  arr->addUse(this);
  idx->addUse(this);
  first = false;
  init = nullptr;
  last = false;
}

void GepInst::output() const {
    Op* dst = operands[0];
    Op* arr = operands[1];
    Op* idx = operands[2];
    std::string arrType = arr->getType()->toStr();
    // Type* type = ((PointerType*)(arr->getType()))->getType();
    // ArrayType* type1 = (ArrayType*)(((ArrayType*)type)->getArrayType());
    // if (type->isInt() || (type1 && type1->getLength() == -1))
    if (paramFirst)
        fprintf(yyout, "  %s = getelementptr inbounds %s, %s %s, i32 %s\n",
                dst->toStr().c_str(),
                arrType.substr(0, arrType.size() - 1).c_str(), arrType.c_str(),
                arr->toStr().c_str(), idx->toStr().c_str());
    else
        fprintf(
            yyout, "  %s = getelementptr inbounds %s, %s %s, i32 0, i32 %s\n",
            dst->toStr().c_str(), arrType.substr(0, arrType.size() - 1).c_str(),
            arrType.c_str(), arr->toStr().c_str(), idx->toStr().c_str());
}

GepInst::~GepInst() {
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

MachineOperand* Inst::genMachineOperand(Op* ope) {
    auto se = ope->getEntry();
    MachineOperand* mope = nullptr;
    if (se->isConst())
        mope = new MachineOperand(
            MachineOperand::IMM,
            dynamic_cast<ConstSymbEntry*>(se)->getValue());
    else if (se->isTemp())
        mope = new MachineOperand(
            MachineOperand::VREG,
            dynamic_cast<TempSymbEntry*>(se)->getLabel());
    else if (se->isVariable()) {
        auto id_se = dynamic_cast<IdSymbEntry*>(se);
        if (id_se->isGlobal())
            mope = new MachineOperand(id_se->toStr().c_str());
        else if (id_se->isParam()) {
            // TODO: 这样分配的是虚拟寄存器 能对应到r0-r3嘛
            //  r4之后的参数需要一条load 哪里加 怎么判断是r4之后的参数
            if (id_se->getParamNo() < 4)
                mope = new MachineOperand(MachineOperand::REG,
                                          id_se->getParamNo());
            else
                // 用r3代表一下
                mope = new MachineOperand(MachineOperand::REG, 3);
        } else
            exit(0);
    }
    return mope;
}

MachineOperand* Inst::genMachineReg(int reg) {
    return new MachineOperand(MachineOperand::REG, reg);
}

MachineOperand* Inst::genMachineVReg() {
    return new MachineOperand(MachineOperand::VREG, SymbTable::getLabel());
}

MachineOperand* Inst::genMachineImm(int val) {
    return new MachineOperand(MachineOperand::IMM, val);
}

MachineOperand* Inst::genMachineLabel(int block_no) {
    std::ostringstream buf;
    buf << ".L" << block_no;
    std::string label = buf.str();
    return new MachineOperand(label);
}

void AllocaInst::genMachineCode(AsmBuilder* builder) {
    /* HINT:
     * Allocate stack space for local variabel
     * Store frame offset in symbol entry */
    auto cur_func = builder->getFunction();
    int size = se->getType()->getSize() / 8;
    if (size < 0)
        size = 4;
    int offset = cur_func->AllocSpace(size);
    dynamic_cast<TempSymbEntry*>(operands[0]->getEntry())
        ->setOffset(-offset);
}

void LoadInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    // Load global operand
    if (operands[1]->getEntry()->isVariable() &&
        dynamic_cast<IdSymbEntry*>(operands[1]->getEntry())
            ->isGlobal()) {
        auto dst = genMachineOperand(operands[0]);
        auto internal_reg1 = genMachineVReg();
        auto internal_reg2 = new MachineOperand(*internal_reg1);
        auto src = genMachineOperand(operands[1]);
        // example: load r0, addr_a
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, src);
        cur_block->InsertInst(cur_inst);
        // example: load r1, [r0]
        cur_inst = new LoadMInstruction(cur_block, dst, internal_reg2);
        cur_block->InsertInst(cur_inst);
    }
    // Load local operand
    else if (operands[1]->getEntry()->isTemp() && operands[1]->getDef() &&
             operands[1]->getDef()->isAlloc()) {
        // example: load r1, [r0, #4]
        auto dst = genMachineOperand(operands[0]);
        auto src1 = genMachineReg(11);
        int off = dynamic_cast<TempSymbEntry*>(operands[1]->getEntry())
                      ->getOffset();
        auto src2 = genMachineImm(off);
        if (off > 255 || off < -255) {
            auto operand = genMachineVReg();
            cur_block->InsertInst(
                (new LoadMInstruction(cur_block, operand, src2)));
            src2 = operand;
        }
        cur_inst = new LoadMInstruction(cur_block, dst, src1, src2);
        cur_block->InsertInst(cur_inst);
    }
    // Load operand from temporary variable
    else {
        // example: load r1, [r0]
        auto dst = genMachineOperand(operands[0]);
        auto src = genMachineOperand(operands[1]);
        cur_inst = new LoadMInstruction(cur_block, dst, src);
        cur_block->InsertInst(cur_inst);
    }
}

void StoreInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineOperand(operands[1]);
    // store immediate
    if (operands[1]->getEntry()->isConst()) {
        auto dst1 = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, dst1, src);
        cur_block->InsertInst(cur_inst);
        // src = dst1;
        src = new MachineOperand(*dst1);
    }
    // store to local
    if (operands[0]->getEntry()->isTemp() && operands[0]->getDef() &&
        operands[0]->getDef()->isAlloc()) {
        auto src1 = genMachineReg(11);
        int off = dynamic_cast<TempSymbEntry*>(operands[0]->getEntry())
                      ->getOffset();
        auto src2 = genMachineImm(off);
        if (off > 255 || off < -255) {
            auto operand = genMachineVReg();
            cur_block->InsertInst(
                (new LoadMInstruction(cur_block, operand, src2)));
            src2 = operand;
        }
        cur_inst = new StoreMInstruction(cur_block, src, src1, src2);
        cur_block->InsertInst(cur_inst);
    }
    // store to global
    else if (operands[0]->getEntry()->isVariable() &&
             dynamic_cast<IdSymbEntry*>(operands[0]->getEntry())
                 ->isGlobal()) {
        auto internal_reg1 = genMachineVReg();
        // example: load r0, addr_a
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, dst);
        cur_block->InsertInst(cur_inst);
        // example: store r1, [r0]
        cur_inst = new StoreMInstruction(cur_block, src, internal_reg1);
        cur_block->InsertInst(cur_inst);
    }
    // store to pointer
    else if (operands[0]->getType()->isPtr()) {
        cur_inst = new StoreMInstruction(cur_block, src, dst);
        cur_block->InsertInst(cur_inst);
    }
}

void BinInst::genMachineCode(AsmBuilder* builder) {
    // complete other instructions
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src1 = genMachineOperand(operands[1]);
    auto src2 = genMachineOperand(operands[2]);
    /* HINT:
     * The source operands of ADD instruction in ir code both can be immediate
     * num. However, it's not allowed in assembly code. So you need to insert
     * LOAD/MOV instrucrion to load immediate num into register. As to other
     * instructions, such as MUL, CMP, you need to deal with this situation,
     * too.*/
    MachineInstruction* cur_inst = nullptr;
    if (src1->isImm()) {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src1);
        cur_block->InsertInst(cur_inst);
        src1 = new MachineOperand(*internal_reg);
    }
    // 合法立即数的判定太复杂 简单判定255以上均load
    // 这里应该也不需要考虑负数
    if (src2->isImm()) {
        if ((opcode <= BinInst::OR &&
             ((ConstSymbEntry*)(operands[2]->getEntry()))->getValue() >
                 255) ||
            opcode >= BinInst::MUL) {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*internal_reg);
        }
    }
    switch (opcode) {
        case ADD:
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::ADD, dst, src1, src2);
            break;
        case SUB:
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::SUB, dst, src1, src2);
            break;
        case AND:
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::AND, dst, src1, src2);
            break;
        case OR:
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::OR,
                                              dst, src1, src2);
            break;
        case MUL:
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::MUL, dst, src1, src2);
            break;
        case DIV:
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::DIV, dst, src1, src2);
            break;
        case MOD: {
            // c = a % b
            // c = a / b
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::DIV, dst, src1, src2);
            MachineOperand* dst1 = new MachineOperand(*dst);
            src1 = new MachineOperand(*src1);
            src2 = new MachineOperand(*src2);
            cur_block->InsertInst(cur_inst);
            // c = c * b
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::MUL, dst1, dst, src2);
            cur_block->InsertInst(cur_inst);
            dst = new MachineOperand(*dst1);
            // c = a - c
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::SUB, dst, src1, dst1);
            break;
        }
        default:
            break;
    }
    cur_block->InsertInst(cur_inst);
}

void CmpInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    auto src1 = genMachineOperand(operands[1]);
    auto src2 = genMachineOperand(operands[2]);
    MachineInstruction* cur_inst = nullptr;
    if (src1->isImm()) {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src1);
        cur_block->InsertInst(cur_inst);
        src1 = new MachineOperand(*internal_reg);
    }
    if (src2->isImm() &&
        ((ConstSymbEntry*)(operands[2]->getEntry()))->getValue() > 255) {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
        cur_block->InsertInst(cur_inst);
        src2 = new MachineOperand(*internal_reg);
    }
    cur_inst = new CmpMInstruction(cur_block, src1, src2, opcode);
    cur_block->InsertInst(cur_inst);
    if (opcode >= CmpInst::L && opcode <= CmpInst::GE) {
        auto dst = genMachineOperand(operands[0]);
        auto trueOperand = genMachineImm(1);
        auto falseOperand = genMachineImm(0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,
                                       trueOperand, opcode);
        cur_block->InsertInst(cur_inst);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,
                                       falseOperand, 7 - opcode);
        cur_block->InsertInst(cur_inst);
    }
}

void UncondBrInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    std::stringstream s;
    s << ".L" << branch->getNo();
    MachineOperand* dst = new MachineOperand(s.str());
    auto cur_inst =
        new BranchMInstruction(cur_block, BranchMInstruction::B, dst);
    cur_block->InsertInst(cur_inst);
}

void CondBrInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    std::stringstream s;
    s << ".L" << true_branch->getNo();
    MachineOperand* dst = new MachineOperand(s.str());
    auto cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B,
                                           dst, cur_block->getCmpCond());
    cur_block->InsertInst(cur_inst);
    s.str("");
    s << ".L" << false_branch->getNo();
    dst = new MachineOperand(s.str());
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, dst);
    cur_block->InsertInst(cur_inst);
}

void RetInst::genMachineCode(AsmBuilder* builder) {
    // TODO
    /* HINT:
     * 1. Generate mov instruction to save return value in r0
     * 2. Restore callee saved registers and sp, fp
     * 3. Generate bx instruction */
    auto cur_block = builder->getBlock();
    if (!operands.empty()) {
        auto dst = new MachineOperand(MachineOperand::REG, 0);
        auto src = genMachineOperand(operands[0]);
        auto cur_inst =
            new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src);
        cur_block->InsertInst(cur_inst);
    }
    auto cur_func = builder->getFunction();
    auto sp = new MachineOperand(MachineOperand::REG, 13);
    auto size =
        new MachineOperand(MachineOperand::IMM, cur_func->AllocSpace(0));
    auto cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,
                                           sp, sp, size);
    cur_block->InsertInst(cur_inst);
    // auto fp = new MachineOperand(MachineOperand::REG, 11);
    // auto cur_inst1 = new StackMInstrcuton(cur_block, StackMInstrcuton::POP,
    //                                       cur_func->getSavedRegs(), fp);
    // cur_block->InsertInst(cur_inst1);
    auto lr = new MachineOperand(MachineOperand::REG, 14);
    auto cur_inst2 =
        new BranchMInstruction(cur_block, BranchMInstruction::BX, lr);
    cur_block->InsertInst(cur_inst2);
}

void CallInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    MachineOperand* operand;  //, *num;
    MachineInstruction* cur_inst;
    // auto fp = new MachineOperand(MachineOperand::REG, 11);
    int idx = 0;
    for (auto it = operands.begin(); it != operands.end(); it++, idx++) {
        if (idx == 0)
            continue;
        if (idx == 5)
            break;
        operand = genMachineReg(idx - 1);
        auto src = genMachineOperand(operands[idx]);
        if (src->isImm() && src->getVal() > 255) {
            cur_inst = new LoadMInstruction(cur_block, operand, src);
        } else
            cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV,
                                           operand, src);
        cur_block->InsertInst(cur_inst);
    }
    for (int i = operands.size() - 1; i > 4; i--) {
        operand = genMachineOperand(operands[i]);
        if (operand->isImm()) {
            auto dst = genMachineVReg();
            if (operand->getVal() < 256)
                cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV,
                                               dst, operand);
            else
                cur_inst = new LoadMInstruction(cur_block, dst, operand);
            cur_block->InsertInst(cur_inst);
            operand = dst;
        }
        std::vector<MachineOperand*> vec;
        cur_inst = new StackMInstrcuton(cur_block, StackMInstrcuton::PUSH, vec,
                                        operand);
        cur_block->InsertInst(cur_inst);
    }
    auto label = new MachineOperand(se->toStr().c_str());
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::BL, label);
    cur_block->InsertInst(cur_inst);
    if (operands.size() > 5) {
        auto off = genMachineImm((operands.size() - 5) * 4);
        auto sp = new MachineOperand(MachineOperand::REG, 13);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,
                                          sp, sp, off);
        cur_block->InsertInst(cur_inst);
    }
    if (dst) {
        operand = genMachineOperand(dst);
        auto r0 = new MachineOperand(MachineOperand::REG, 0);
        cur_inst =
            new MovMInstruction(cur_block, MovMInstruction::MOV, operand, r0);
        cur_block->InsertInst(cur_inst);
    }
}

void ZextInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineOperand(operands[1]);
    auto cur_inst =
        new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src);
    cur_block->InsertInst(cur_inst);
}

void XorInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto trueOperand = genMachineImm(1);
    auto falseOperand = genMachineImm(0);
    auto cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,
                                        trueOperand, MachineInstruction::EQ);
    cur_block->InsertInst(cur_inst);
    cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,
                                   falseOperand, MachineInstruction::NE);
    cur_block->InsertInst(cur_inst);
}

void GepInst::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;
    auto dst = genMachineOperand(operands[0]);
    auto idx = genMachineOperand(operands[2]);
    if(init){
        if(last){
            auto base = genMachineOperand(init);
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::ADD, dst, base, genMachineImm(4));
            cur_block->InsertInst(cur_inst);
        }
        return;
    }
    MachineOperand* base = nullptr;
    int size;
    auto idx1 = genMachineVReg();
    if (idx->isImm()) {
        if (idx->getVal() < 255) {
            cur_inst =
                new MovMInstruction(cur_block, MovMInstruction::MOV, idx1, idx);
        } else {
            cur_inst = new LoadMInstruction(cur_block, idx1, idx);
        }
        idx = new MachineOperand(*idx1);
        cur_block->InsertInst(cur_inst);
    }
    if (paramFirst) {
        size =
            ((PointerType*)(operands[1]->getType()))->getType()->getSize() / 8;
    } else {
        if (first) {
            base = genMachineVReg();
            if (operands[1]->getEntry()->isVariable() &&
                ((IdSymbEntry*)(operands[1]->getEntry()))
                    ->isGlobal()) {
                auto src = genMachineOperand(operands[1]);
                cur_inst = new LoadMInstruction(cur_block, base, src);
            } else {
                int offset = ((TempSymbEntry*)(operands[1]->getEntry()))
                                 ->getOffset();
                if (offset > -255 && offset < 255) {
                    cur_inst =
                        new MovMInstruction(cur_block, MovMInstruction::MOV,
                                            base, genMachineImm(offset));
                } else {
                    cur_inst = new LoadMInstruction(cur_block, base,
                                                    genMachineImm(offset));
                }
            }
            cur_block->InsertInst(cur_inst);
        }
        ArrayType* type =
            (ArrayType*)(((PointerType*)(operands[1]->getType()))->getType());
        size = type->getElementType()->getSize() / 8;
    }
    auto size1 = genMachineVReg();
    if (size > -255 && size < 255) {
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, size1,
                                       genMachineImm(size));
    } else {
        cur_inst = new LoadMInstruction(cur_block, size1, genMachineImm(size));
    }
    cur_block->InsertInst(cur_inst);
    auto off = genMachineVReg();
    cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, off,
                                      idx, size1);
    off = new MachineOperand(*off);
    cur_block->InsertInst(cur_inst);
    if (paramFirst || !first) {
        auto arr = genMachineOperand(operands[1]);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,
                                          dst, arr, off);
        cur_block->InsertInst(cur_inst);
    } else {
        auto addr = genMachineVReg();
        auto base1 = new MachineOperand(*base);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,
                                          addr, base1, off);
        cur_block->InsertInst(cur_inst);
        addr = new MachineOperand(*addr);
        if (operands[1]->getEntry()->isVariable() &&
            ((IdSymbEntry*)(operands[1]->getEntry()))->isGlobal()) {
            cur_inst =
                new MovMInstruction(cur_block, MovMInstruction::MOV, dst, addr);
        } else {
            auto fp = genMachineReg(11);
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::ADD, dst, fp, addr);
        }
        cur_block->InsertInst(cur_inst);
    }
}