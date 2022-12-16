#include "Unit.h"

void Unit::insertFunc(Func* f) {
  func_list.push_back(f);
}

void Unit::insertId(SymbEntry* id) {
  for (const auto& i : id_list) {
    if (((IdSymbEntry*) id)->getName() == ((IdSymbEntry*) i)->getName()) return;
  }
  id_list.push_back(id);
}

void Unit::removeFunc(Func* func) {
  func_list.erase(std::find(func_list.begin(), func_list.end(), func));
}

void Unit::output() const {
  for (const auto& id : id_list) ((IdSymbEntry*) id)->output();
  for (const auto& func : func_list) func->output();
}

Unit::~Unit() {
  for (auto& id : id_list) delete id;
  for (auto& func : func_list) delete func;
}
