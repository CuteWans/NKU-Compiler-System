#ifndef __UNIT_H__
#define __UNIT_H__

#include <vector>
#include "Function.h"

class Unit {
  typedef std::vector<Func*>::iterator iterator;
  typedef std::vector<Func*>::reverse_iterator reverse_iterator;

private:
  std::vector<Func*> func_list;
  std::vector<SymbEntry*> id_list;

public:
  Unit() = default;
  ~Unit();
  void insertFunc(Func*);
  void insertId(SymbEntry*);
  void removeFunc(Func*);
  void output() const;

  iterator begin() {
    return func_list.begin();
  }

  iterator end() {
    return func_list.end();
  }

  reverse_iterator rbegin() {
    return func_list.rbegin();
  }

  reverse_iterator rend() {
    return func_list.rend();
  }
};

#endif