#include <list>
#include "Function.h"
#include "Type.h"
#include "Unit.h"

extern FILE* yyout;

Func::Func(Unit* u, SymbEntry* s) {
  u->insertFunc(this);
  entry = new BasicBlock(this);
  sym_ptr = s;
  parent = u;
}

Func::~Func() {
  auto delete_list = block_list;
  for (auto& i : delete_list) delete i;
  parent->removeFunc(this);
}

// remove the basicblock bb from its block_list.
void Func::remove(BasicBlock* bb) {
  block_list.erase(std::find(block_list.begin(), block_list.end(), bb));
}

void Func::output() const {
  FuncType* funcType = dynamic_cast<FuncType*>(sym_ptr->getType());
  Type* retType = funcType->getRetType();
  fprintf(
    yyout, "define %s %s(", retType->toStr().c_str(), sym_ptr->toStr().c_str());
  auto param = params.begin();
  if (param == params.end()) {
    //no param
    fprintf(yyout, "){\n");
  } else {
    fprintf(yyout, "%s %s", (*param)->getType()->toStr().c_str(),
      (*param)->toStr().c_str());
    ++param;
    for (; param != params.end(); ++param) {
      fprintf(yyout, ",%s %s", (*param)->getType()->toStr().c_str(),
        (*param)->toStr().c_str());
    }
    fprintf(yyout, "){\n");
  }

  std::set<BasicBlock*> v;
  std::list<BasicBlock*> q;
  q.push_back(entry);
  v.insert(entry);
  while (!q.empty()) {
    auto bb = q.front();
    q.pop_front();
    bb->output();
    for (auto succ = bb->succ_begin(); succ != bb->succ_end(); ++succ) {
      if (v.find(*succ) == v.end()) {
        v.insert(*succ);
        q.push_back(*succ);
      }
    }
  }
  fprintf(yyout, "}\n");
}
