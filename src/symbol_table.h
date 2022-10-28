#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
#include <string.h>

#include "list.h"
#include "util.h"

#define MX_ID_LENGTH 31

typedef struct {
  unsigned     type;
  char         name[MX_ID_LENGTH + 1];
  list_entry_t list_entry;
  unsigned     extra[0];
} symbol_t;

static inline int symbol_empty(symbol_t* tab) {
  return tab == NULL || list_empty(&tab->list_entry);
}

static symbol_t* symbol_init() {
  symbol_t* symb = malloc(sizeof(symbol_t));
  list_init(&symb->list_entry);
  return symb;
}

static inline void symbol_insert(symbol_t* tab, symbol_t* elm) {
  list_insert(&tab->list_entry, &elm->list_entry);
}

static inline void symbol_remove(symbol_t* elm) {
  list_remove(&elm->list_entry);
}

static inline void symbol_print_name(symbol_t* tab) {
  if (symbol_empty(tab)) return;

  list_entry_t* bound = &tab->list_entry;
  list_entry_t* i     = list_next(bound);
  list_entry_t* j;

  while (i != bound) {
    j = list_next(i);

    symbol_t* symb = tostruct(i, symbol_t, list_entry);
    puts(symb->name);

    i = j;
  }
}

static symbol_t* symbol_get(symbol_t* tab, const char* name) {
  if (symbol_empty(tab)) return NULL;

  list_entry_t* bound = &tab->list_entry;
  list_entry_t* i     = list_next(bound);
  list_entry_t* j;

  symbol_t* symb = tab;
  while (i != bound) {
    j = list_next(i);

    symb = tostruct(i, symbol_t, list_entry);
    if (strcmp(symb->name, name) == 0) break;

    i = j;
  }
  if (i == bound) return NULL;
  list_remove(&symb->list_entry);
  list_insert(&tab->list_entry, &symb->list_entry);
  return symb;
}

static void symbol_destory(symbol_t* tab) {
  list_entry_t* bound = &tab->list_entry;
  list_entry_t* i     = bound;
  list_entry_t* j;
  do {
    j = list_next(i);

    symbol_t* symb = tostruct(i, symbol_t, list_entry);
    free(symb);

    i = j;
  } while (i != bound);
}

#endif  // _SYMBOL_TABLE_H_