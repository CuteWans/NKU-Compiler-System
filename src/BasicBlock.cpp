#include <algorithm>
#include "BasicBlock.h"
#include "Function.h"

extern FILE* yyout;

// insert the instruction to the front of the basicblock.
void BasicBlock::insertFront(Inst* inst) {
  insertBefore(inst, head->getNext());
}

// insert the instruction to the back of the basicblock.
void BasicBlock::insertBack(Inst* inst) {
  insertBefore(inst, head);
}

// insert the instruction dst before src.
void BasicBlock::insertBefore(Inst* dst, Inst* src) {
  // Todo
  src->getPrev()->setNext(dst);
  dst->setPrev(src->getPrev());
  dst->setNext(src);
  src->setPrev(dst);
  dst->setParent(this);
}

// remove the instruction from intruction list.
void BasicBlock::remove(Inst* inst) {
  inst->getPrev()->setNext(inst->getNext());
  inst->getNext()->setPrev(inst->getPrev());
}

void BasicBlock::output() const {
  fprintf(yyout, "B%d:", no);

  if (!pred.empty()) {
    fprintf(yyout, "%*c; preds = %%B%d", 32, '\t', pred[0]->getNo());
    for (auto it = pred.begin() + 1; it != pred.end(); ++it)
      fprintf(yyout, ", %%B%d", (*it)->getNo());
  }
  fprintf(yyout, "\n");
  for (auto i = head->getNext(); i != head; i = i->getNext()) i->output();
}

void BasicBlock::addSucc(BasicBlock* bb) {
  succ.push_back(bb);
}

// remove the successor basicclock bb.
void BasicBlock::removeSucc(BasicBlock* bb) {
  succ.erase(std::find(succ.begin(), succ.end(), bb));
}

void BasicBlock::addPred(BasicBlock* bb) {
  pred.push_back(bb);
}

// remove the predecessor basicblock bb.
void BasicBlock::removePred(BasicBlock* bb) {
  pred.erase(std::find(pred.begin(), pred.end(), bb));
}

BasicBlock::BasicBlock(Func* f) {
  this->no = SymbTable::getLabel();
  f->insertBlock(this);
  parent = f;
  head = new DummyInst();
  head->setParent(this);
}

BasicBlock::~BasicBlock() {
  Inst* inst;
  inst = head->getNext();
  while (inst != head) {
    Inst* t;
    t = inst;
    inst = inst->getNext();
    delete t;
  }
  for (auto& bb : pred) bb->removeSucc(this);
  for (auto& bb : succ) bb->removePred(this);
  parent->remove(this);
}

void BasicBlock::genMachineCode(AsmBuilder* builder) 
{
    auto cur_func = builder->getFunction();
    auto cur_block = new MachineBlock(cur_func, no);
    builder->setBlock(cur_block);
    for (auto i = head->getNext(); i != head; i = i->getNext())
    {
        i->genMachineCode(builder);
    }
    cur_func->InsertBlock(cur_block);
}
