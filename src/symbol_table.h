#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
#include "list.h"
typedef struct {
  unsigned     type;
  list_entry_t list_entry;
  unsigned     extra[0];
} symbol_t;
#endif  // _SYMBOL_TABLE_H_