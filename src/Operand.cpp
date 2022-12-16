#include <string.h>
#include <algorithm>
#include <sstream>
#include "Operand.h"

std::string Op::toStr() const {
  return se->toStr();
}

void Op::removeUse(Inst* inst) {
  auto i = std::find(uses.begin(), uses.end(), inst);
  if (i != uses.end()) uses.erase(i);
}
